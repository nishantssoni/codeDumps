from langchain_core.messages import HumanMessage
from langchain_ollama import ChatOllama  # FIX: Updated import path

# Initialize ChatOllama with your local 0.8b model
chat = ChatOllama(model="qwen3.5:0.8b")

print("Sending local request to local Ollama (qwen3.5:0.8b)...")

try:
    response = chat.invoke([
        HumanMessage(content="hi!! how are you?")
    ])
    
    print("\nResponse:")
    print(response.content)
except Exception as e:
    print(f"\nAn error occurred: {e}")
    print("Ensure the Ollama application is active in your taskbar tray!")