import os
from typing import Optional


def _get_provider() -> str:
    return os.getenv("LLM_PROVIDER", "gemini").strip().lower()


def generate_text(prompt: str, *, gemini_model: Optional[str] = None, openai_model: Optional[str] = None) -> str:
    provider = _get_provider()

    if provider in ("openai", "oai"):
        api_key = os.getenv("OPENAI_API_KEY")
        if not api_key:
            raise ValueError("OPENAI_API_KEY environment variable is not set")

        model = os.getenv("OPENAI_MODEL") or openai_model or "gpt-4.1-mini"
        try:
            from openai import OpenAI
        except Exception as exc:
            raise ImportError("openai package is required for LLM_PROVIDER=openai") from exc

        client = OpenAI(api_key=api_key)
        response = client.responses.create(
            model=model,
            input=prompt,
        )
        return (response.output_text or "").strip()

    api_key = os.getenv("GEMINI_API_KEY")
    if not api_key:
        raise ValueError("GEMINI_API_KEY environment variable is not set")

    model = os.getenv("GEMINI_MODEL") or gemini_model or "gemini-2.5-pro"
    try:
        import google.generativeai as genai
    except Exception as exc:
        raise ImportError("google-generativeai package is required for LLM_PROVIDER=gemini") from exc

    genai.configure(api_key=api_key)
    llm = genai.GenerativeModel(model)
    response = llm.generate_content(prompt)
    if not response or not response.parts:
        return ""
    return (response.text or "").strip()
