from langchain_core.messages import HumanMessage
from langchain_community.chat_models import ChatOllama

chat = ChatOllama(model="deepseek-r1:latest")

response = chat.invoke([
    HumanMessage(content="What's the capital of France?")
])
print(response.content)
