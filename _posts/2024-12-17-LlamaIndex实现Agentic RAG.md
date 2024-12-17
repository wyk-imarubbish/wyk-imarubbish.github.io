---
title: LlamaIndex实现Agentic RAG
tags:
  - 大模型
  - RAG
---

> 参考：[【RAG检索增强生成】LlamaIndex与Qwen2的高效检索增强生成实践_llamaindex qwen2-CSDN博客](https://blog.csdn.net/xiaobing259/article/details/140593905)

## 前期准备工作

1. 安装必要的依赖

```bash
pip install modelscope
pip install llama-index
pip install llama-index-llms-huggingface
pip install llama-index-readers-web
pip install llama-index-core
pip install llama-index-llms-openai
pip install llama-index-llms-replicate
pip install llama-index-embeddings-huggingface
```

> 如果安装速度过慢，可能是没有换源。

2. 下载语言模型

```python
from modelscope import snapshot_download

# snapshot_download函数用于下载模型
model_dir = snapshot_download(
    'qwen/Qwen2-7B-Instruct',  # 模型名称
    cache_dir='./model',  # 缓存目录
    revision='master'  # 版本号
)
```

> 下载过程可能出现问题，可以在modelscope官网手动下载然后传到对应文件夹下

3. 下载文本嵌入模型

```python
from modelscope import snapshot_download

# snapshot_download函数用于下载模型
model_dir = snapshot_download(
    'AI-ModelScope/bge-base-zh-v1.5',  # 模型名称
    cache_dir='./model',  # 缓存目录
    revision='master'  # 版本号
)
```

## 模型设置

> 在之前下载的模型中，文件名中的小数点会自动变为___对应地要在代码中进行更改。

```py
# Set prompt template for generation (optional)
from llama_index.core import PromptTemplate

def completion_to_prompt(completion):
   return f"<|im_start|>system\n<|im_end|>\n<|im_start|>user\n{completion}<|im_end|>\n<|im_start|>assistant\n"

def messages_to_prompt(messages):
    prompt = ""
    for message in messages:
        if message.role == "system":
            prompt += f"<|im_start|>system\n{message.content}<|im_end|>\n"
        elif message.role == "user":
            prompt += f"<|im_start|>user\n{message.content}<|im_end|>\n"
        elif message.role == "assistant":
            prompt += f"<|im_start|>assistant\n{message.content}<|im_end|>\n"

    if not prompt.startswith("<|im_start|>system"):
        prompt = "<|im_start|>system\n" + prompt

    prompt = prompt + "<|im_start|>assistant\n"

    return prompt


# Set Qwen2 as the language model and set generation config
Settings.llm = HuggingFaceLLM(
    model_name="./model/qwen/Qwen2___5-3B-Instruct",
    tokenizer_name="./model/qwen/Qwen2___5-3B-Instruct",
    context_window=30000,
    max_new_tokens=2000,
    generate_kwargs={"temperature": 0.7, "top_k": 50, "top_p": 0.95},
    messages_to_prompt=messages_to_prompt,
    completion_to_prompt=completion_to_prompt,
    device_map="auto",
)

# Set embedding model
Settings.embed_model = HuggingFaceEmbedding(
    model_name = "./model/AI-ModelScope/bge-base-zh-v1.5"
)
```

## 构建索引

1. 设置分块大小, 以优化检索效率

```python
# Set the size of the text chunk for retrieval
Settings.transformations = [SentenceSplitter(chunk_size=1024)]
```

2. 文档内容索引构建

```python
from llama_index.core import VectorStoreIndex, SimpleDirectoryReader
documents = SimpleDirectoryReader("./data").load_data()
index = VectorStoreIndex.from_documents(
    documents,
    embed_model=Settings.embed_model,
    transformations=Settings.transformations
)
```

3. 网站内容构建索引

```python
from llama_index.readers.web import SimpleWebPageReader
from llama_index.core import VectorStoreIndex, SimpleDirectoryReader

documents = SimpleWebPageReader(html_to_text=True).load_data(
    ["web_address_1","web_address_2",...]
)
index = VectorStoreIndex.from_documents(
    documents,
    embed_model=Settings.embed_model,
    transformations=Settings.transformations
)
```

## 问答示例

```python
query_engine = index.as_query_engine()
your_query = "你是谁？"
print(query_engine.query(your_query).response)
```



