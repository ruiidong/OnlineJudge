OnlineJudge
===========================

## 环境依赖
1. jsoncpp
2. ctemplate

## 部署步骤
1. 克隆项目到本地
   ```git clone github.com/ruiidong/OnlineJudge.git```

2. 进入项目目录
	```cd OnlineJudge```

3. 生成makefile文件
	```cmake .```

4. 生成可执行文件
	```make```

5. 启动OnlineJudge
	```cd bin```
	```./OnlineJudge```

## 目录结构描述
	├── bin 可执行文件
	├── build 中间文件
	├── CMakeLists.txt 编译配置文件
	├── include 头文件
	├── lib 库文件
	├── mysql.ini 数据库配置文件
	├── README.md 项目描述文件
	├── sample 题目样例
	├── src 源文件
	│   ├── db 数据库连接
	│   ├── model ORM
	│   └── tinymuduo 根据Muduo网络库改写的HTTP服务器
	│		├── base 与网络无关的基础代码
	│		└── net 网络库
	├── temp 临时文件
	└── template 网页html模板