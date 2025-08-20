from langchain_huggingface import ChatHuggingFace, HuggingFacePipeline
import torch

# Clear GPU memory if available
if torch.cuda.is_available():
    torch.cuda.empty_cache()

# Create the pipeline
llm = HuggingFacePipeline.from_model_id(
    model_id='TinyLlama/TinyLlama-1.1B-Chat-v1.0',
    task='text-generation',
    device=0,  # Use CPU to avoid memory issues
    model_kwargs={
        "torch_dtype": torch.float16,  # Use float32 for CPU
    },
    pipeline_kwargs={
        "temperature": 0.5,
        "max_new_tokens": 100,
        "do_sample": True,  # Enable sampling for temperature to work
    }
)

# Create ChatHuggingFace with explicit model_id
model = ChatHuggingFace(
    llm=llm,
    model_id='TinyLlama/TinyLlama-1.1B-Chat-v1.0'  # Explicitly specify model_id
)

# Test the model
print(torch.cuda.is_available())
result = model.invoke("What is the capital of India?")
print(result.content)