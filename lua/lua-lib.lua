lua的常用库：string；math；table；io;基本库
forever.z整理收集！
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
lua字符串库 http://blog.csdn.net/nmn0317/article/details/4933207

-lua中字符串索引从前往后是1,2,……，从后往前是-1，-2……。
--string库中所有的function都不会直接操作字符串，只返回一个结果。

【基本函数】
函数									描述													示例								结果
len								计算字符串长度									string.len("abcd")						4
rep(s,n)						返回字符串s的n个拷贝								string.rep("abcd",2)					abcdabcd
lower							变小写											string.lower("AbcD")					abcd
upper							变大写											string.upper("AbcD")					ABCD
format							返回一个类似printf的格式化字符串					string.format("the value is:%d",4)		the value is:4
																
																				string.sub("abcd",2)					bcd
sub(s,i,j)					returns substring from index i to j of s			string.sub("abcd",-2)					cd
																				string.sub("abcd",2,-2)					bc
																				string.sub("abcd",2,3)					bc

								查找pattern在s中的位置,返回pattern在				string.find("cdcdcdcd","ab")			nil
find(s,pattern,init,plain)		s中的开始位置和结束位置.							string.find("cdcdcdcd","cd")			1 2
								init是开始查找的位置.plain终止位置					string.find("cdcdcdcd","cd",7)			7 8

gsub(s, pattern, reps)			s源字符串pattern代替换							string.gsub("abcdabcd","a","z");		zbcdzbcd 2
								替换为reps，返回结果串和匹配数						string.gsub("aaaa","a","z",3);			zzza 3

byte(string,pos)				返回第pos个字符的整数表示形式						string.byte("ABCD",4)					68
char(1,2,...,100)				将整型数字转成字符并连接							string.char(97,98,99,100)				abcd
string.dump(functoin)			返回一个参数函数的2进制代码
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
string.gfind(s, pattern)	返回一个迭代器，迭代器每执行一次，返回下一个匹配串；
例如：iter = string.gfind("a=b c=d", "[^%s+]=[^%s+]")							print(iter()) <== a=b 		print(iter()) <== c=d
通常用于泛性for循环,下面的例子结果同上
for s in string.gfind("a=b c=d", "[^%s+]=[^%s+]") do
print(s)
end
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
字符类：
. 任意字符	%s 空白符	%p 标点字符	%c 控制字符	%d 数字	%x 十六进制数字	%z 代表0的字符（表示没有）	%a 字母  %l 小写字母	%u 大写字母	%w 字母和数字
上面字符类的大写形式表示小写所代表的集合的补集。例如，'%A'非字母的字符：

'%' 用作特殊字符的转义字符		'%.' 匹配点；		'%%' 匹配字符 '%'。
转义字符 '%'不仅可以用来转义特殊字符，还可以用于所有的非字母的字符。当对一个字符有疑问的时候，为安全起见请使用转义字符转义他。

用'[]'创建字符集						'[%w_]' 匹配字母数字和下划线				'[01]' 匹配二进制数字						'[%[%]]'匹配一对方括号
在'[]'中使用连字符'-'					'%d'    表示 '[0-9]'；					'%x'    表示 '[0-9a-fA-F]'				'[0-7]' 表示 '[01234567]'
在'[]'开始处使用 '^' 表示其补集：		'[^0-7]' 匹配任何不是八进制数字的字符；		'[^/n]' 匹配任何非换行符户的字符。			'[^%s]' == '%S'
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
string.gmatch(s,pattern)	返回一个迭代函数，每次调用此函数，将返回下一个查找到的样式串对应的字符；用法同上gfind。

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
lua数学库 http://www.cnblogs.com/whiteyun/archive/2009/08/10/1543040.html

math.pi 为圆周率常量 = 3.14159265358979323846
abs				取绝对值															math.abs(-15)					15
acos			反余弦函数														math.acos(0.5)					1.04719755
asin			反正弦函数														math.asin(0.5)					0.52359877
atan2			x / y的反正切值													math.atan2(90.0, 45.0)			1.10714871
atan			反正切函数														math.atan(0.5)					0.463647609
ceil			不小于x的最大整数													math.ceil(5.8)					6
cosh			双曲线余弦函数													math.cosh(0.5)					1.276259652
cosh			余弦函数															math.cos(0.5)					0.87758256
deg				弧度转角度														math.deg(math.pi)				180
exp				计算以e为底x次方值													math.exp(2)						2.718281828
floor			不大于x的最大整数													math.floor(5.6)					5
fmod （mod）		取模运算															math.fmod(14, 5)					4
frexp			把双精度数val分解为数字部分（尾数）和以2为底的指数n，即val=x*2n			math.frexp(10.0)				0.625 4
ldexp			计算value * 2的n次方												math.ldexp(10.0, 3)				80 = 10 * (2 ^3)
log10			计算以10为基数的对数												math.log10(100)					2
log				计算一个数字的自然对数												math.log(2.71)					0.9969
max				取得参数中最大值													math.max(2.71, 100, -98, 23)	100
min				取得参数中最小值													math.min(2.71, 100, -98, 23)	-98
modf			把数分为整数和小数													math.modf(15.98)				15 98
pow				得到x的y次方														math.pow(2, 5)					32
rad				角度转弧度														math.rad(180)					3.14159265358
random			获取随机数														math.random(1, 100)				获取1-100的随机数
																				math.random(100)				获取1-100的随机数
randomseed		设置随机数种子													math.randomseed(os.time())		在使用math.random函数之前必须使用此函数设置随机数种子
sinh			双曲线正弦函数													math.sinh(0.5)					0.5210953
sin				正弦函数															math.sin(math.rad(30))			0.5
sqrt			开平方函数														math.sqrt(16)					4
tanh			双曲线正切函数													math.tanh(0.5)					0.46211715
tan				正切函数															math.tan(0.5)					0.5463024

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
lua.table库 http://www.cnblogs.com/whiteyun/archive/2009/08/10/1543139.html

table.concat(table, sep,  start, end)
	concat是concatenate(连锁, 连接)的缩写. table.concat()函数列出参数中指定table的数组部分从start位置到end位置的所有元素, 元素间以指定的分隔符(sep)隔开。除了table外, 其他的参数都不是必须的, 分隔符的默认值是空字符, start的默认值是1, end的默认值是数组部分的总长.
	sep, start, end这三个参数是顺序读入的, 所以虽然它们都不是必须参数, 但如果要指定靠后的参数, 必须同时指定前面的参数.

table.insert(table, pos, value)
	table.insert()函数在table的数组部分指定位置(pos)插入值为value的一个元素. pos参数可选, 默认为数组部分末尾.

table.maxn(table)
	table.maxn()函数返回指定table中所有正数key值中最大的key值. 如果不存在key值为正数的元素, 则返回0. 【此函数不限于table的数组部分】

table.remove(table, pos)
	table.remove()函数删除并返回table数组部分位于pos位置的元素. 其后的元素会被前移. pos参数可选, 默认为table长度, 即从最后一个元素删起.

table.sort(table, comp)
	table.sort()函数对给定的table进行升序排序.
	comp是一个可选的参数, 此参数是一个外部函数, 可以用来自定义sort函数的排序标准.
	此函数应满足以下条件: 接受两个参数(依次为a, b), 并返回一个布尔型的值, 当a应该排在b前面时, 返回true, 反之返回false.

table.foreachi(table, function(i, v))
	会期望一个从 1（数字 1）开始的连续整数范围，遍历table中的key和value逐对进行function(i, v)操作

table.foreach(table, function(i, v))
	与foreachi不同的是，foreach会对整个表进行迭代

table.getn(table)
	返回table中元素的个数

table.setn(table, nSize)
	设置table中的元素个数

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
lua.io库http://blog.csdn.net/duanxuyun/article/details/4751118

一、io表调用方式：使用io表，io.open将返回指定文件的描述，并且所有的操作将围绕这个文件描述
　　io表同样提供三种预定义的文件描述io.stdin,io.stdout,io.stderr

二、文件句柄直接调用方式,即使用file:XXX()函数方式进行操作,其中file为io.open()返回的文件句柄
　　多数I/O函数调用失败时返回nil加错误信息,有些函数成功时返回nil

具体函数：
1、io.close ([file])					功能：相当于file:close()，关闭默认的输出文件
2、io.flush ()						功能：相当于file:flush(),输出所有缓冲中的内容到默认输出文件
3、io.lines ([filename])				功能：打开指定的文件filename为读模式并返回一个迭代函数,每次调用将获得文件中的一行内容,当到文件尾时，将返回nil,并自动关闭文件	
	若不带参数时io.lines() <=> io.input():lines(); 读取默认输入设备的内容，但结束时不关闭文件
4、io.open (filename [, mode])	功能：按指定的模式打开一个文件，成功则返回文件句柄，失败则返回nil+错误信息
	mode:		"r": 读模式 (默认);
				"w": 写模式;
				"a": 添加模式;
				"r+": 更新模式，所有之前的数据将被保存
				"w+": 更新模式，所有之前的数据将被清除
				"a+": 添加更新模式，所有之前的数据将被保存,只允许在文件尾进行添加
				"b": 某些系统支持二进制方式
5、io.output ([file])				功能：相当于io.input，但操作在默认输出文件上
6、io.popen ([prog [, mode]])		功能：开始程序prog于额外的进程,并返回用于prog的文件句柄(并不支持所有的系统平台)
7、io.read (...)						功能：相当于io.input():read
8、io.tmpfile ()						功能：返回一个临时文件句柄，该文件以更新模式打开，程序结束时自动删除
9、io.type (obj)						功能：检测obj是否一个可用的文件句柄
	返回：   	"file"：为一个打开的文件句柄
				"closed file"：为一个已关闭的文件句柄
				nil:表示obj不是一个文件句柄
10、io.write (...)					功能：相当于io.output():write
11、file:close()						功能：关闭文件	注：当文件句柄被垃圾收集后，文件将自动关闭。句柄将变为一个不可预知的值
12、file:flush()						功能：向文件写入缓冲中的所有数据
13、file:lines()						功能：返回一个迭代函数,每次调用将获得文件中的一行内容,当到文件尾时，将返回nil,但不关闭文件
14、file:read(...)					功能：按指定的格式读取一个文件,按每个格式函数将返回一个字串或数字,如果不能正确读取将返回nil,若没有指定格式将指默认按行方式进行读取
　　格式：　		"*n": 读取一个数字
				"*a": 从当前位置读取整个文件,若为文件尾，则返回空字串
				"*l": [默认]读取下一行的内容,若为文件尾，则返回nil
				number: 读取指定字节数的字符,若为文件尾，则返回nil;如果number为0则返回空字串,若为文件尾，则返回nil;
15、file:seek([whence][,offset])		功能：设置和获取当前文件位置,成功则返回最终的文件位置(按字节),失败则返回nil加错误信息
　　参数whence:		"set": 从文件头开始
					"cur": 从当前位置开始[默认]
					"end": 从文件尾开始
　　offset:默认为0	不带参数file:seek()则返回当前位置,file:seek("set")则定位到文件头,file:seek("end")则定位到文件尾并返回文件大小
16、file:setvbuf(mode,[,size])		功能：设置输出文件的缓冲模式
　　参数mode:			"no": 没有缓冲，即直接输出
					"full": 全缓冲，即当缓冲满后才进行输出操作(也可调用flush马上输出)
					"line": 以行为单位，进行输出(多用于终端设备)
　　最后两种模式,size可以指定缓冲的大小(按字节)，忽略size将自动调整为最佳的大小
17、file:write(...)					功能：按指定的参数格式输出文件内容，参数必须为字符或数字，若要输出其它值，则需通过tostring或string.format进行转换
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
基本库 http://www.cnblogs.com/whiteyun/archive/2009/08/12/1543184.html

1、assert (v [, message])
功能：相当于C的断言，
参数：
v：当表达式v为nil或false将触发错误,
message：发生错误时返回的信息，默认为"assertion failed!"
2、collectgarbage (opt [, arg])
功能：是垃圾收集器的通用接口，用于操作垃圾收集器
参数：
opt：操作方法标志
"Stop": 停止垃圾收集器 
"Restart": 重启垃圾收集器 
"Collect": 执行一次全垃圾收集循环
"Count": 返回当前Lua中使用的内存量(以KB为单位)
"Step": 单步执行一个垃圾收集. 步长 "Size" 由参数arg指定　(大型的值需要多步才能完成)，如果要准确指定步长，需要多次实验以达最优效果。如果步长完成一次收集循环，将返回True
"Setpause": 设置 arg/100 的值作为暂定收集的时长 
"Setstepmul": 设置 arg/100 的值，作为步长的增幅(即新步长＝旧步长*arg/100)
3、dofile (filename)
功能：打开并且执行一个lua块,当忽略参数filename时，将执行标准输入设备(stdin)的内容。返回所有块的返回值。当发生错误时，dofile将错误反射给调用者
注：dofile不能在保护模式下运行
4、error (message [, level])
功能：终止正在执行的函数，并返回message的内容作为错误信息(error函数永远都不会返回)
通常情况下，error会附加一些错误位置的信息到message头部.
Level参数指示获得错误的位置,
Level=1[默认]：为调用error位置(文件+行号)
Level=2：指出哪个调用error的函数的函数
Level=0:不添加错误位置信息
5、_G全局环境表(全局变量)
功能：记录全局环境的变量值的表 _G._G = _G
6、getfenv(f)
功能：返回函数f的当前环境表
参数：f可以为函数或调用栈的级别，级别1[默认]为当前的函数,级别0或其它值将返回全局环境_G
7、getmetatable(object)
功能：返回指定对象的元表(若object的元表.__metatable项有值，则返回object的元表.__metatable的值)，当object没有元表时将返回nil
8、ipairs (t)
功能：返回三个值 迭代函数、表、0
多用于穷举表的键名和键值对
如：for i,v in ipairs(t) do 
end
每次循环将索引赋级i，键值赋给v
注：本函数只能用于以数字索引访问的表 如：t={"1","cash"}
9、load (func [, chunkname])
功能：装载一个块中的函数，每次调用func将返回一个连接前一结的字串，在块结尾处将返回nil
当没有发生错误时，将返回一个编译完成的块作为函数,否则返回nil加上错误信息，此函数的环境为全局环境
chunkname用于错误和调试信息
10、load (func [, chunkname])
功能：装载一个块中的函数，每次调用func将返回一个连接前一结的字串，在块结尾处将返回nil
当没有发生错误时，将返回一个编译完成的块作为函数,否则返回nil加上错误信息，此函数的环境为全局环境
chunkname用于错误和调试信息
11、loadfile ([filename])
功能：与load类似，但装载的是文件或当没有指定filename时装载标准输入(stdin)的内容
12、loadstring (string [, chunkname])
功能：与load类似，但装载的内容是一个字串
如：assert(loadstring(s))()
13、next (table [, index])
功能：允许程序遍历表中的每一个字段，返回下一索引和该索引的值。
参数：table：要遍历的表
　　　index：要返回的索引的前一索中的号，当index为nil[]时，将返回第一个索引的值，当索引号为最后一个索引或表为空时将返回nil
注：可以用next(t)来检测表是否为空(此函数只能用于以数字索引的表与ipairs相类似)
14、ipairs (t)
功能：返回三个值 next函数、表、0
多用于穷举表的键名和键值对
如：for n,v in pairs(t) do 
end
每次循环将索引赋级i，键值赋给v
注：本函数只能用于以键名索引访问的表 如：t={id="1",name="cash"}
15、pcall (f, arg1, ···)
功能：在保护模式下调用函数(即发生的错误将不会反射给调用者)
当调用函数成功能返回true,失败时将返回false加错误信息
16、print (···)						功能：简单的以tostring方式格式化输出参数的内容
17、rawequal (v1, v2)				功能：检测v1是否等于v2，此函数不会调用任何元表的方法
18、rawget (table, index)
功能：获取表中指定索引的值，此函数不会调用任何元表的方法，成功返回相应的值，当索引不存在时返回nil
注：本函数只能用于以数字索引访问的表 如：t={"1","cash"}
19、rawset (table, index, value)
功能：设置表中指定索引的值，此函数不会调用任何元表的方法，此函数将返回table
20、select (index, ···)
功能：当index为数字将返回所有index大于index的参数:如：select(2,"a","b") 返回 "b"
当index为"#"，则返回参数的总个数(不包括index)
21、setfenv (f, table)
功能：设置函数f的环境表为table
参数：f可以为函数或调用栈的级别，级别1[默认]为当前的函数,级别0将设置当前线程的环境表
22、setmetatable (table, metatable)
功能：为指定的table设置元表metatable，如果metatable为nil则取消table的元表，当metatable有__metatable字段时，将触发错误
注：只能为LUA_TTABLE 表类型指定元表
23、tonumber (e , base)
功能：将参数e转换为数字(10进制)，当不能转换时返回nil
base(2~36)指出参数e当前使用的进制，默认为10进制，如tonumber(11,2)=3
24、tostirng(e)						功能：将参数e转换为字符串，此函数将会触发元表的__tostring事件
25、type(v)							功能：返回参数的类型名("nil"，"number", "string", "boolean", "table", "function", "thread", "userdata")
26、unpack (list [, i [, j]])
功能：返回指定表的索引的值,i为起始索引，j为结束索引
注：本函数只能用于以数字索引访问的表,否则只会返回nil 如：t={"1","cash"}
27、_VERSION							功能：返回当前Lua的版本号"Lua 5.1". 
28、xpcall (f, err)
功能：与pcall类似，在保护模式下调用函数(即发生的错误将不会反射给调用者)
但可指定一个新的错误处理函数句柄
当调用函数成功能返回true,失败时将返回false加err返回的结果


