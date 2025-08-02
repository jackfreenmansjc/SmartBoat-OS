import os
from langchain_community.chat_models import ChatZhipuAI
from langchain.schema.messages import HumanMessage, SystemMessage
from dotenv import load_dotenv
load_dotenv()

# 设置代理（如果你是国内用户需要这个）
os.environ['http_proxy'] = 'http://127.0.0.1:7897'
os.environ['https_proxy'] = 'http://127.0.0.1:7897'

chat = ChatZhipuAI(model="glm-4",api_key="eef4e4fc90844695aacaad77187b53bf.JAZQq7FHvUq9Gs6y")
# 创建消息列表
messages = [
    SystemMessage(content="你是一个乐于助人的智能助手。"),
    HumanMessage(content="你好，帮我写一段Python代码实现快速排序。")
]
result = chat.invoke(messages)
print(result.content)