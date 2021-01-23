# SelectStockFromFunds
根据基金持仓选取股票小程序 by max

--- 

## 选股小程序  使用指南    

* 环境： Windows10 

* 按照Stock.txt中的格式，存储你想要的筛选的基金和对应的十大持仓 
* 程序会根据指定的最大权重（maxValue，即第一位持仓股票的权重）和最小权重（minValue，即第十位持仓股票的权重），
   为每个持仓中的股票计算该股票权重。最终，统计所有持仓中的某只股票得分
* 注意将Stock.txt文件放在与 SelectStock.exe 同目录下 
* 注意Stock.txt中内容如果直接复制自excel，需要修改该文件格式： 
   记事本打开该文件，选择文件，选择另存为，在下侧编码选择“ANSI”，然后另存为新的Stock.txt文件即可 

* 程序运行结束会保存新的Stock_Score.txt文件 

* 如何运行？    
   双击Run.bat文件即可，如果你想修改最大和最小权重，右键点击Run.bat，选择编辑，在第一行修改数值参数即可，然后保存
   重新执行Run.bat
