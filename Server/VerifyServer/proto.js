/*
关键字,     能否重新赋值 (Reassignment),能否修改内部属性 (Mutation),作用域
var,        可以,                      ✅ 可以,                  函数作用域（已过时）
let,        可以,                      ✅ 可以,                  块级作用域
const,      不可以,                    ✅ 可以（针对对象/数组）,   块级作用域
*/
const path = require("path")
const grpc = require("@grpc/grpc-js")
const proto_loder = require("@grpc/proto-loader")

const PROTO_PATH = path.join(__dirname,"message.proto")

//它可以是一个普通的字符串单个文件路径,也可以是一个字符串数组多个文件路径
//options?: proto_loader.Options
//? 表示这是一个可选参数
//proto_loader.Options定义了参数的具体“长相”

const package_definition = proto_loder.loadSync(PROTO_PATH,{ 
                                                keepCase: true, //如果为 true，则保留字段名的原始大小写
                                                longs: String, //控制如何表示 Protocol Buffers 中的 long 类型。如果设置为 String，则长整数会被转换为字符串，以避免 JavaScript 中的整数溢出问题。
                                                enums: String, //控制如何表示 Protocol Buffers 中的枚举类型。如果设置为 String，则枚举值会被转换为字符串
                                                defaults: true, //如果为 true，则为未明确设置的字段提供默认值。
                                                oneofs: true })//如果为 true，则支持 Protocol Buffers 中的 oneof 特性。
const proto_descriptor = grpc.loadPackageDefinition(package_definition)
const message_proto = proto_descriptor.message
module.exports = message_proto
//SQVJm9WCFjyafwBu