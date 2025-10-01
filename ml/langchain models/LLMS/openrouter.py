from openai import OpenAI
# load env
from dotenv import load_dotenv
load_dotenv()

client = OpenAI(
  base_url="https://openrouter.ai/api/v1",
)

completion = client.chat.completions.create(
  model="openai/o4-mini",
  messages=[
    {
      "role": "user",
      "content": "What is the capital of france?"
    }
  ]
)
print(completion.choices[0].message.content)