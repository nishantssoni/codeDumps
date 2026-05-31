import os
from dotenv import load_dotenv
from langchain_openai import OpenAIEmbeddings

# Load environment variables from your .env file
load_dotenv()

# Initialize LangChain client settings exactly as before
embeddings = OpenAIEmbeddings(
    model="nvidia/llama-nemotron-embed-vl-1b-v2:free",
    openai_api_key=os.getenv("OPENROUTER_API_KEY"),
    openai_api_base="https://openrouter.ai/api/v1",
)

# Set up the OpenRouter Multimodal Demo Input payload
multimodal_input = [
    {
        "content": [
            {"type": "text", "text": "What is in this image?"},
            {
                "type": "image_url", 
                "image_url": {"url": "https://live.staticflickr.com/3851/14825276609_098cac593d_b.jpg"}
            }
        ]
    }
]

print("Generating multimodal embeddings by bypassing string checks...")

try:
    # FIX: Bypass LangChain string validation by accessing the native client layout
    response = embeddings.client.create(
        model=embeddings.model,
        input=multimodal_input,
        encoding_format="float"
    )
    
    # Extract the vectors from the response payload
    vector = response.data[0].embedding
    
    print("\nSuccess!")
    print(f"Embedding Vector Length (Dimensions): {len(vector)}")
    print(f"First 5 numbers of the vector slice: {vector[:5]}")
    
except Exception as e:
    print(f"\nAn error occurred: {e}")