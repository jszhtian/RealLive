RL引擎加密

根据毛子RW英文化，Xmoe的PCK封包分析，kdays Azure的封包分析。
已知RealLive有俩个解密key。解密方式为XOR 8位长数据所在地址,key。key执行中储存在AL寄存器中。
Xor逆运算直接XOR即可。
第一个Key  16*8bit(Mov AL,字符串  MOV ebp+??,AL)（8bit操作）
第二个key 128*8bit(MOV AL,ECX+字符串)【2个文件不相同！！】

文本存在2次加密！
解密值=原始值XOR当前文本序号*0x7087（XOR CX,ebp+0x10）目标寄存器CX！（16bit操作）


部分代码来自项目https://github.com/number201724/SiglusEngine

文本UTF16LE编码结构

Gameexe.dat结构（2次XOR解密后）

+0x8 head区
+0x8 除head区文件大小，解压后大小
->eof 压缩数据段


Scene.pck结构(2次XOR解密前)
+0x4   head标识;
+0x8	 varInfo;
+0x8	 varNameIndex;
+0x8	 varName;
+0x8	 cmdInfo;
+0x8	 cmdNameIndex;
+0x8	 cmdName;
+0x8	 SceneNameIndex;
+0x8	 SceneName;
+0x8	 SceneInfo;
+0x8	 SceneData;
+0x4	 ExtraKeyUse;
+0x4	 SourceHeaderLength;

NameIdex->文件名长度
Name->文件名
info->文件起点/zhongdian
Data->文件（加密）
SceneData区文本加密！
Data 2次XOR解密后，4bit/4bit 原始大小（包含8bit），解压后大小（不包含8bit）//查汇编代码！
解压不包括data区解密后前8bit解压信息区
内联汇编 入口1 解压源 入口2 解压缓存 入口3解压后长度

Script原始文件

0x4 headerLength;
0x8 unknow;
0x8 string_index;
0x8 string_data;
后面不管。。。

详细定义
typedef struct ScriptHdr{
        DWORD ScriptHdrLen;
        DWORD OperationOffset;
        DWORD OperationCount;
        DWORD StringTableOffset;
        DWORD StringTableCount;
        DWORD OriginalStringOffset;
        DWORD OriginalStringCount;
        DWORD Var1PairOffset;
        DWORD Var1PairCount;
        DWORD Var2PairOffset;
        DWORD Var2PairCount;
        DWORD VarArray[17];
        DWORD Var3PairOffset;
        DWORD Var3PairCount;
        DWORD Var4PairOffset;
        DWORD Var4PairCount;
};

index->offset，length
data->需要解密  看前面！
