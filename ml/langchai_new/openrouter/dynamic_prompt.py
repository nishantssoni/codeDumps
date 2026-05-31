import os
import streamlit as st
from dotenv import load_dotenv
from langchain_openai import ChatOpenAI
from langchain_core.prompts import PromptTemplate

# Load your OPENROUTER_API_KEY from the .env file
load_dotenv()

# Verify API key is present before continuing
api_key = os.getenv("OPENROUTER_API_KEY")

# Initialize the OpenRouter client using ChatOpenAI
# Added extra headers that OpenRouter heavily recommends to avoid 401/403 or blank drops
llm = ChatOpenAI(
    # model="meta-llama/llama-3.1-8b-instruct",
    model="openrouter/free",
    openai_api_key=api_key,
    openai_api_base="https://openrouter.ai/api/v1",
    temperature=0.3,
    default_headers={
        "HTTP-Referer": "http://localhost:8501", # Required by OpenRouter rankings
        "X-Title": "Research Paper Summarizer",  # Shows up in your OpenRouter dashboard
    }
)

st.title("📄 Research Paper Summarizer (Cloud Edition)")

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
    value=200, 
    step=50
)

# A button to trigger the summarization
if st.button("Generate Summary"):
    if not api_key:
        st.error("Missing OPENROUTER_API_KEY. Please check your .env file.")
    elif paper_input:
        with st.spinner("Connecting to OpenRouter and generating summary..."):
            try:
                # Structure the prompt template cleanly
                template = PromptTemplate.from_template(
                    "Please summarize the research paper titled \"{paper}\" with the following specifications:\n\n"
                    "- Explanation Style: {style}\n"
                    "- Explanation Length: Approximately {length} words\n\n"
                    "Summary Specifications:\n"
                    "1. Mathematical Details: Include relevant mathematical equations if present in the paper. Explain the concepts using simple code snippets where applicable.\n"
                    "2. Analogies: Use relatable analogies to simplify complex ideas.\n\n"
                    "Crucial Instruction: If certain information is not available in the paper, you MUST respond with the exact phrase: \"Insufficient information available\" instead of guessing."
                )

                try:
                    # Attempt to format the prompt
                    prompt = template.format(
                        paper=paper_input,
                        style=style_input,
                        length=length_input
                    )
                    
                except KeyError as e:
                    # This triggers if a variable name is misspelled or missing
                    st.error(f"Prompt Validation Error: Missing expected variable {e}")
                    st.stop() # Prevents Streamlit from running the LLM call below it

                except Exception as e:
                    st.error(f"An unexpected prompt error occurred: {e}")
                    st.stop()

                # If it gets past the block above, your prompt is 100% successfully validated
                result = llm.invoke(prompt)
                summary_result = result.content

                # Check if the response actually contains data
                if summary_result and summary_result.strip():
                    # Display the results on the UI screen
                    st.subheader("Generated Summary")
                    st.success(f"Summary for '{paper_input}' generated successfully!")
                    st.write(summary_result)
                else:
                    st.warning("The model responded, but the output text was empty. Check your OpenRouter credits/logs.")

            except Exception as e:
                st.error(f"An error occurred: {e}")
    else:
        st.error("Please enter a research paper title to proceed.")