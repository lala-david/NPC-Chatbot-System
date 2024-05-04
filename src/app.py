import os
from flask import Flask, request, jsonify
from flask_cors import CORS
from langchain.chat_models import ChatOpenAI
from langchain.document_loaders import PyMuPDFLoader
from langchain.embeddings import OpenAIEmbeddings
from langchain.prompts import PromptTemplate, SystemMessagePromptTemplate, HumanMessagePromptTemplate, ChatPromptTemplate
from langchain.text_splitter import SpacyTextSplitter
from langchain.vectorstores import Chroma
from langchain.chains import RetrievalQA
import numpy as np

app = Flask(__name__)
CORS(app)

embeddings = OpenAIEmbeddings(model="text-embedding-3-small")

text_splitter = SpacyTextSplitter(chunk_size=200, pipeline="ko_core_news_sm")
documents = PyMuPDFLoader("last.pdf").load()
splitted_documents = text_splitter.split_documents(documents)

persist_directory = "./.data"
if not os.path.exists(persist_directory):
    os.makedirs(persist_directory)

database = Chroma.from_documents(
    documents=splitted_documents,
    embedding=embeddings,
    persist_directory=persist_directory
)
database.persist()

system_prompt = SystemMessagePromptTemplate(prompt=PromptTemplate(
    template="""
    반말만 해! 너는 골렘이고 엄청 성격도 쎈 친구야!: {context}
    """,
    input_variables=["context"]
))

human_prompt = HumanMessagePromptTemplate(prompt=PromptTemplate(
    template="{context}", 
    input_variables=["context"]
))

chat_prompt = ChatPromptTemplate.from_messages([system_prompt, human_prompt])

llm = ChatOpenAI(model_name="gpt-3.5-turbo", temperature=1)

retriever = database.as_retriever(search_type="similarity")
qa = RetrievalQA.from_chain_type(
    llm=llm,
    chain_type='map_reduce',
    retriever=retriever,
    return_source_documents=True
)

@app.route('/chat', methods=['POST'])
def chat_with_npc():
    user_input = request.json.get("message", "")
    if not isinstance(user_input, str):
        return jsonify({"error": "잘못된 입력 했습니다."}), 400

    query_vector = embeddings.embed_query(user_input)
    result = qa({"query": user_input})
    response = result.get("result", "잘 모르겠어!")

    document_vectors = []
    for doc in result.get('source_documents', []):
        try:
            doc_text = doc.page_content
            doc_vector = embeddings.embed_query(doc_text)
            document_vectors.append(doc_vector)
        except AttributeError as e:
            print(f"Error accessing text of document: {e}")

    similarities = [np.dot(query_vector, doc_vec) / (np.linalg.norm(query_vector) * np.linalg.norm(doc_vec)) for doc_vec in document_vectors]

    similarity_threshold = 0.2  
    if all(sim < similarity_threshold for sim in similarities):
        response = "잘 모르겠어 그런 말은 처음 들어봤어"

    for i, sim in enumerate(similarities, start=1):
        print(f"문서 {i}와 질문의 코사인 유사도: {sim:.2f}")

    return jsonify({"response": response})

if __name__ == "__main__":
    app.run(host="", port=0000, debug=True)