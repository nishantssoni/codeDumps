import os
from dotenv import load_dotenv
from langchain_core.messages import HumanMessage
from langchain_openai import ChatOpenAI

# Load environment variables from your .env file
load_dotenv()

# Initialize the ChatOpenAI client with the universal free router
chat = ChatOpenAI(
    model="openrouter/free",  # FIX: Automatically selects an active free model
    openai_api_key=os.getenv("OPENROUTER_API_KEY"),
    openai_api_base="https://openrouter.ai/api/v1",
    temperature=0.7,
)

print("Sending request to OpenRouter using auto-free router...")

try:
    response = chat.invoke([
        HumanMessage(content="Write a five line poem.")
    ])
    print("\nResponse:")
    print(response.content)
except Exception as e:
    print(f"\nAn error occurred: {e}")