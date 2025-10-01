import streamlit as st
import time
from openai import OpenAI
from dotenv import load_dotenv
load_dotenv()

client = OpenAI(
  base_url="https://openrouter.ai/api/v1",
)

st.title("📄 Research Paper Summarizer")

# Input for Paper Title
paper_input = st.selectbox(
    "Paper Input",
    ["Attention is all you need", "Word2Vec", "Transformer", "BERT", "GPT-3", "GPT-4", "OpenAI", "ChatGPT", "DALL-E", "Stable Diffusion"]
)

# Input for Explanation Style
style_input = st.selectbox(
    "Explanation Style",
    ["Academic", "Concise", "Detailed", "Simple", "Technical"]
)

# Input for Explanation Length
length_input = st.slider(
    "Explanation Length (in words)",
    min_value=50,
    max_value=500,
    value=200, # Default value
    step=50
)

st.subheader("Summary Specifications:")



# A button to trigger the summarization (backend logic would go here)
if st.button("Generate Summary"):
    if paper_input:
        with st.spinner("Generating summary..."):
             # 1. Construct the detailed prompt from the UI inputs
            prompt = f"""
            Please summarize the research paper titled "{paper_input}" with the following specifications:

            - Explanation Style: {style_input}
            - Explanation Length: Approximately {length_input} words

            Summary Specifications:
            1.  **Mathematical Details:**
                -   Include relevant mathematical equations if present in the paper.
                -   Explain the mathematical concepts using simple, intuitive code snippets where applicable.
            2.  **Analogies:**
                -   Use relatable analogies to simplify complex ideas.

            **Crucial Instruction:** If certain information is not available in the paper, you MUST respond with the exact phrase: "Insufficient information available" instead of guessing.

            Ensure the summary is clear, accurate, and aligned with the provided style and length.
            """

            # 2. Call the language model with the constructed prompt
            try:
                completion = client.chat.completions.create(
                  model="openai/o4-mini", # Or any other model
                  messages=[
                    {
                      "role": "user",
                      "content": prompt
                    }
                  ]
                )

                # 3. Display the result from the model
                summary_result = completion.choices[0].message.content
                st.subheader("Generated Summary")
                st.success(f"Summary for '{paper_input}' will be generated with style '{style_input}' and length '{length_input}' words.")
                st.write(summary_result)

            except Exception as e:
                st.error(f"An error occurred: {e}")

    else:
        st.error("Please enter a research paper title to proceed.")