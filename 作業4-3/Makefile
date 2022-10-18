#設定一個存放資料夾路徑的變數
SECTIONS = \
	code/asm \
	code/os \


#告訴make,當沒有特別提到要做什麼事情時，預設要執行的任務
.DEFAULT_GOAL := all


#宣告all任務要做的事
all :
#	echo的意思是顯示echo這條命令本身及其輸出結果,在前面加一個@的意思是不要顯示命令本身，只顯示結果
	@echo "begin compile ALL exercises for assembly samples ......................."

#	for dir的意思是說,對SECTIONS變數中儲存的每個路徑執行do之後描述的動作 $(MAKE)代表make這個命令
#   $$dir 第一個$代表擴展的功能，意思是說，make會把指令處理成 
for dir in code/os; do make -C $dir || exit "$?"; done  再交給shell處理，這樣就能取用shell的 $dir、$?變數
#	所以就是對每個資料夾中的.c檔進行make，done就是等全部做完再進行下一步
	for dir in $(SECTIONS); do $(MAKE) -C $$dir || exit "$$?"; done
#	顯示echo命令中所包含的語句	
	@echo "compile ALL exercises finished successfully! ......"

#告訴make clean是偽目標，不要當作編譯來執行
.PHONY : clean 

#設定一個clean偽目標，對每個資料夾執行專屬於它的clean偽目標
clean:
	for dir in $(SECTIONS); do $(MAKE) -C $$dir clean || exit "$$?"; done


#告訴make slides目標是偽目標，不要當作編譯來執行
.PHONY : slides

#設定一個slides偽目標，刪除所有slides資料夾中的PDF，用soffice指令將 ./docs/ppts/路徑下的PPT檔轉為PDF，放入 ./slides資料夾中
slides:
	rm -f ./slides/*.pdf
	soffice --headless --convert-to pdf:writer_pdf_Export --outdir ./slides ./docs/ppts/*.pptx



