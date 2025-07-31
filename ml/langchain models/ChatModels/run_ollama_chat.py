from langchain_ollama import ChatOllama

llm = ChatOllama(model="llama3.2:1b")
result = llm.invoke("What is the capital of India?")
print(result.content)