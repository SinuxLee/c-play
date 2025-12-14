
### vscode-like 识别 xmake 安装的 pacakge path
```bash
# 生成 .vscode/compile_commands.json
xmake project -k compile_commands .vscode

# ctr/cmd + p 然后输入 >C/C++: Edit Configurations (JSON)
# 在 c_cpp_properties.json 中添加
"compileCommands": "${workspaceFolder}/.vscode/compile_commands.json"

# 在 settings.json 中添加
"C_Cpp.default.compileCommands": "${workspaceFolder}/.vscode/compile_commands.json"
```

### 嵌入式编译器
- Keil
- SDCC
- GCC
- IAR
