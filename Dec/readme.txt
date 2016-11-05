RL引擎加密

根据毛子RW英文化，Xmoe的PCK封包分析，kdays Azure的封包分析。
已知RealLive有俩个解密key。解密方式为XOR 8位长数据所在地址,key。key执行中储存在AL寄存器中。
Xor逆运算直接XOR即可。
第一个Key  16*8bit(Mov AL,字符串  MOV ebp+??,AL)
第二个key 128*8bit(MOV AL,ECX+字符串)
