# Arthas

## 项目结构

| 项目名称 | 项目类型 | 说明|
|----|----|----|
| iplugin | library | 为多插件支持(暂时忽略) |
| plugin_impl | library | 插件具体实现模块 |
| plugin_impl_apk | application | 依赖plugin_impl打包成apk插件文件 |
| plugin_loader | library | 插件加载模块, c++实现DexClassLoader动态加载 |
| plugin_pack | library | 打包模块, 依赖loader库和plugin_impl_apk文件, 其他代码和资源 |
| sample | application | sample模块, 依赖plugin_pack库, 测试plugin_pack和plugin_impl_apk所有功能demo项目 |

## 运行流程

直接运行sample的run，打包apk安装测试，sample利用gradle的插件，自动打包plugin_impl_apk项目到assets目录下做插件

## 插件的实现

插件入口类`wow.arthas.plugin.PluginImpl`, 入口方法`void onCreate(Context context);`

## 支持功能

- [x] plugin调用host
- [x] host调用plugin
- [x] 


## 补充

### plugin调用host

接入流程:

1. host 依赖plugin_pack库，并打包进app
2. plugin_impl 只编译期依赖plugin_pack库，并不打包进插件apk文件内
3. 调用的方法注意不能混淆


### host调用plugin

由于ClassLoader不同, host不能直接调用plugin中的类和方法, 需要获取到plugin中的DexClassLoader, 由此ClassLoader的loadClass,
然后反射出Method对象来调用

host提供回调接口, plugin注册回调接口, host调用plugin设置的callback, 来达到host调用plugin方法
