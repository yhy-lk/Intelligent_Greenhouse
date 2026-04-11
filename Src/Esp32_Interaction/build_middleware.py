Import("env")

def skip_lvgl_asm(node):
    # 将节点转换为绝对路径字符串
    path = str(node)
    # 如果是 .S 汇编文件，且路径中包含了 lvgl，直接抛弃不编译
    if path.endswith(".S") and "lvgl" in path:
        return None
    return node

# 注册中间件，接管所有编译节点的分配
env.AddBuildMiddleware(skip_lvgl_asm)