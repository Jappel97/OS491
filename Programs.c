#include "frame.h"
#include "pi.h"
#include "base.h"
#include "Font.h"
#include "string.h"
#include "Programs.h"
#include "memory.h"
#include "string.h"
#include "math.h"
#include "univals.h"

#define SOLVE 0
#define MENU  1
#define GRAPH 2
#define SPECIAL 3
#define ENTRY 4

unsigned char shift;


//This is what will be running most of the time.
void shell(void){
	unsigned int MODE = SOLVE;
	shift = 0;
	unsigned int menuType = 7;
	unsigned int rc = 0, rd = 0;
	menu currMen = { .set=0, .numTabs=0, .tabs = 0x00};
	int currTab = 0, currEntry = 0;
	XSCALE = 20;
	YSCALE = 20;
	queue funcs[5];
	char* inputBuffer = malloc(80 * sizeof(char));
	inputBuffer[0] = '\0';
	int i = 0;
	for(i = 0; i < 5; i++){
		funcs[i] = (queue){.tail = 0};
	}
	for(i = 0; i < 2; i++){
		for(int j = 0; j < 256; j++){
			lists[i][j] = (thing){.type = 0, .value = 0};
		}
	}
	for(i = 0; i < 26; i++){
		vars[i] = (KVPair){.key = i + 65, .value = 0};
	}
	while(1){
		rd = rc;
		rc = getInput();
		if(rc != 0 && rc != rd){
			if((keyCodes[rc])[0] == 'Q'){
				MODE = SOLVE;
				FBCharReset();
				GFXColor(0x00);
			}
			if(MODE == SOLVE){
				SolveInputHandler(rc, inputBuffer);
			}
			else if(MODE == MENU){
				MenuInputHandler(rc, &currMen, &currTab, &currEntry);
			}
			else if(MODE == GRAPH){
				GraphInputHandler(rc);
			}
			else if(MODE == ENTRY){
				EntryInputHandler(rc, inputBuffer, funcs);
			}
		}
		if(MODE == SOLVE){
			//TODO
			if(((keyCodes[rc])[0] == 'V' || (keyCodes[rc])[0] == 'S' 
			|| (keyCodes[rc])[0] == 'M')
			 && shift == 0){
				MODE = MENU;
				menuType = (keyCodes[rc])[0];
			}
			else if((keyCodes[rc])[0] == 'G'){
				GFXColor(0x00);
				MODE = GRAPH;
				i = 0;
			}
			else if((keyCodes[rc][0] == 'Y')){
				GFXColor(0x00);
				FBWriteString("Y=", 2);
				MODE = ENTRY;
			}
		}
		else if(MODE == MENU){
			//TODO
			if(currMen.set == 0){
				GFXColor(0x00);
				if(menuType == 'M'){
					mathMenuInit(&currMen);
				}
				else if(menuType == 'V'){
					varsMenuInit(&currMen);
				}
				else if(menuType == 'S'){
					statMenuInit(&currMen);
				}
				else if(menuType == 'Y'){
					funcMenuInit(&currMen);
				}
				else{
					MODE = SOLVE;
					FBCharReset();
				}
			}
			else{
				showEntries(&currMen, currTab, currEntry);
			}
		}
		else if(MODE == GRAPH){
			if(i < 5){
				if(funcs[i].tail > 0){
					graph(funcs[i], XSCALE, YSCALE);
				}
				i++;
			}
		}
		else if(MODE == ENTRY){
			if(rc == 33){
				MODE = SOLVE;
				FBCharReset();
			}
		}
		else if (MODE == SPECIAL){
			//This is for special functions such as entering lists or functions
			//This will instantly return to SOLVE mode as soon as "=" is pressed
			//We thus need globals for these???
			//A global for each of 5 functions
			//And two globals for two lists
			//A list, from which we will derive data for our stats functions
			//Should probably be.... a queue of max size 255. An entry will have mode 1, while an empty entry has mode 0.
			//Thus, when counting through a list when we first reach mode 0, we stop.
			
		}
		else{
			MODE = SOLVE;
			FBCharReset();
		}
        FBRender();
	}
}

void SolveInputHandler(unsigned char input, char* inputBuffer){
	if(shift == 0){
		if((input%12 > 8 || input%12 == 0) || input == 37 || (input == 20)|| (input == 32) || ((input%12 < 7 && input%12 >4) && input > 12) ){
			if(input == 33){
				queue RPN = INFtoRPN(inputBuffer, SOLVE);
				double result = RPNResolve(RPN);
				FBCharNewline();
				FBWriteChar('=');
				//FBWriteString(inputBuffer, strlen(inputBuffer));
				FBWriteDec(result);
				inputBuffer[0] = '\0';
				FBCharNewline();
			}
			else if(input == 20){
				FBWriteString("SIN(", 4);
				strcharcat(inputBuffer, (keyCodes[input])[0]);
			}
			else if(input == 32){
				FBWriteString("LOG(", 4);
				strcharcat(inputBuffer, (keyCodes[input])[0]);
			}
			else if(input == 18){
				FBWriteString("LN(", 3);
				strcharcat(inputBuffer, (keyCodes[input])[0]);
			}
			else if(input == 17){
				FBWriteString("COS(", 4);
				strcharcat(inputBuffer, (keyCodes[input])[0]);
			}
			else if(input == 29){
				FBWriteString("TAN(", 4);
				strcharcat(inputBuffer, (keyCodes[input])[0]);
			}
			else{
				FBWriteChar((keyCodes[input])[0]);
				strcharcat(inputBuffer, (keyCodes[input])[0]);
			}
		}
	}
	if(shift != 1){
		shift = 0;
	}
	return;
}

void MenuInputHandler(unsigned char input, menu* currentMenu, int* currTab, int* currEntry){
	if(input == 15 && *currTab > 0){
		*currTab -= 1;
		*currEntry = 0;
	}
	else if(input == 27 && *currTab < currentMenu->numTabs){
		*currTab += 1;
		*currEntry = 0;
	}
	else if(input == 16 && *currEntry > 0){
		*currEntry -= 1;
	}
	else if(input == 28 && *currTab < currentMenu->tabs[*currTab].numEntries){
		*currEntry += 1;
	}
	else if(input == 33){
		menuEntry(currentMenu->tabs[*currTab].entries[*currEntry]);
	}
	return;
}

void GraphInputHandler(unsigned char input){
	
}

void EntryInputHandler(unsigned char input, char* inputBuffer, queue* funcs){
	if(shift == 0){
		if((input%12 > 8 || input%12 == 0) || input == 37 || (input == 20)|| (input == 32) || ((input%12 < 7 && input%12 >4) && input > 12) || (input == 40) ){
			if(input == 33){
				queue RPN = INFtoRPN(inputBuffer, ENTRY);
				funcs[0] = RPN;
			}
			else if(input == 20){
				FBWriteString("SIN(", 4);
				strcharcat(inputBuffer, (keyCodes[input])[0]);
			}
			else if(input == 32){
				FBWriteString("LOG(", 4);
				strcharcat(inputBuffer, (keyCodes[input])[0]);
			}
			else if(input == 18){
				FBWriteString("LN(", 3);
				strcharcat(inputBuffer, (keyCodes[input])[0]);
			}
			else if(input == 17){
				FBWriteString("COS(", 4);
				strcharcat(inputBuffer, (keyCodes[input])[0]);
			}
			else if(input == 29){
				FBWriteString("TAN(", 4);
				strcharcat(inputBuffer, (keyCodes[input])[0]);
			}
			else{
				FBWriteChar((keyCodes[input])[0]);
				strcharcat(inputBuffer, (keyCodes[input])[0]);
			}
		}
	}
	if(shift != 1){
		shift = 0;
	}
	return;
}

void mathMenuInit(menu* men){
	clearMenu(men);
	men->set = 1;
	men->numTabs = 3;
	men->tabs = malloc(3 * sizeof(menuTab));
	for(int i = 0; i < men->numTabs; i++){
		men->tabs[i].numEntries = 10;
		men->tabs[i].entries = (char**)malloc(men->tabs[i].numEntries * sizeof(char*));
		for(int j = 0; j < men->tabs[i].numEntries; j++){
			men->tabs[i].entries[j] = "entry";
		}
	}
	int tabWidth = (maxHeight) / (men->numTabs);
	int tabBase = 30; //Since 1 character is 10 pixels tall
	for(int i = 0; i < men->numTabs; i++){
		for(int j = 0; j < tabBase; j++){
			FBWrite((i+1) * tabWidth, j, 0xFF);
		}
	}
	FBCharReset();
	FBCharNewline();
	int tabChars = tabWidth/8; //8 pixels per char, so we find out how many chars per tab based on the width of the tab.
	for(int i = 0; i < men->numTabs; i++){
		for(int j = 0; j < tabChars; j++){
			if(j == 1){
				FBWriteChar('A');
			}
			else{
				FBCharAdvance();
			}
		}
	}
	for(int i = 0; i < maxHeight; i++){
		FBWrite(i, tabBase, 0xFF);
	}
}


//This is the menu in which the user will set all kinds of variables
//This includes not only variables A-Z, but also opperating variables.
void varsMenuInit(menu* men){
	clearMenu(men);
	men->set = 1;
	men->numTabs = 2;
	men->tabs = malloc(2 * sizeof(menuTab));
	for(int i = 0; i < men->numTabs; i++){
		men->tabs[i].numEntries = 10;
		men->tabs[i].entries = (char**)malloc(men->tabs[i].numEntries * sizeof(char*));
		for(int j = 0; j < men->tabs[i].numEntries; j++){
			men->tabs[i].entries[j] = "entry";
		}
	}
	int tabWidth = (maxHeight) / (men->numTabs);
	int tabBase = 30; //Since 1 character is 10 pixels tall
	for(int i = 0; i < men->numTabs; i++){
		for(int j = 0; j < tabBase; j++){
			FBWrite((i+1) * tabWidth, j, 0xFF);
		}
	}
	FBCharReset();
	FBCharNewline();
	int tabChars = tabWidth/8; //8 pixels per char, so we find out how many chars per tab based on the width of the tab.
	for(int i = 0; i < men->numTabs+1; i++){
		for(int j = 0; j < tabChars; j++){
			if(j == 1){
				FBWriteChar('A');
			}
			else{
				FBCharAdvance();
			}
		}
	}
	for(int i = 0; i < maxHeight; i++){
		FBWrite(maxHeight, tabBase, 0xFF);
	}
}

void statMenuInit(menu* men){
	clearMenu(men);
	men->set = 1;
	men->numTabs = 3;
	men->tabs = malloc(3 * sizeof(menuTab));
	for(int i = 0; i < men->numTabs; i++){
		men->tabs[i].numEntries = 10;
		men->tabs[i].entries = (char**)malloc(men->tabs[i].numEntries * sizeof(char*));
		for(int j = 0; j < men->tabs[i].numEntries; j++){
			men->tabs[i].entries[j] = "entry";
		}
	}
		int tabWidth = (maxHeight) / (men->numTabs);
	int tabBase = 30; //Since 1 character is 10 pixels tall
	for(int i = 0; i < men->numTabs; i++){
		for(int j = 0; j < tabBase; j++){
			FBWrite((i+1) * tabWidth, j, 0xFF);
		}
	}
	FBCharReset();
	FBCharNewline();
	int tabChars = tabWidth/8; //8 pixels per char, so we find out how many chars per tab based on the width of the tab.
	for(int i = 0; i < men->numTabs+1; i++){
		for(int j = 0; j < tabChars; j++){
			if(j == 1){
				FBWriteChar('A');
			}
			else{
				FBCharAdvance();
			}
		}
	}
	for(int i = 0; i < maxHeight; i++){
		FBWrite(maxHeight, tabBase, 0xFF);
	}
}

void funcMenuInit(menu* men){
	clearMenu(men);
	men->set = 1;
	men->numTabs = 1;
	men->tabs = malloc(3 * sizeof(menuTab));
	for(int i = 0; i < men->numTabs; i++){
		men->tabs[i].numEntries = 5;
		men->tabs[i].entries = (char**)malloc(men->tabs[i].numEntries * sizeof(char*));
		for(int j = 0; j < men->tabs[i].numEntries; j++){
			men->tabs[i].entries[j] = "Y=";
		}
	}
	int tabWidth = (maxHeight) / (men->numTabs);
	int tabBase = 30; //Since 1 character is 10 pixels tall
	for(int i = 0; i < men->numTabs; i++){
		for(int j = 0; j < tabBase; j++){
			FBWrite((i+1) * tabWidth, j, 0xFF);
		}
	}
	FBCharReset();
	FBCharNewline();
	int tabChars = tabWidth/8; //8 pixels per char, so we find out how many chars per tab based on the width of the tab.
	for(int i = 0; i < men->numTabs+1; i++){
		for(int j = 0; j < tabChars; j++){
			if(j == 1){
				FBWriteChar('A');
			}
			else{
				FBCharAdvance();
			}
		}
	}
	for(int i = 0; i < maxHeight; i++){
		FBWrite(maxHeight, tabBase, 0xFF);
	}
}

void clearMenu(menu* men){
	//We are not set
	men->set=0;
	//Clear out each tab
	for(int i = 0; i < men->numTabs; i++){
		/*
		//clear each entry in the tab
		for(int j = 0; j < men->tabs[i].numEntries; j++){
			free(men->tabs[i].entries[j], 8 * sizeof(char));
		}
		* */
		//We have no entries
		free(men->tabs[i].entries, men->tabs[i].numEntries);
		men->tabs[i].numEntries = 0;
	}
	//Remove the pointer to the tabs
	free(men->tabs, men->numTabs * sizeof(menuTab));
	//we have no tabs
	men->numTabs = 0;
}

//P  - 03
//E  - 02
//MD - 01
//AS - 00
//Functions can be represented as any non-capital letter, or any symbol not already used for an op.
//

//trig functions are treated as special parens which get put in the queue from the stack
//1 + 2 * sin(3 + 4)
//3 4 + sin 2 * 1 +
queue INFtoRPN(char* infix, int MODE){
	int length = strlen(infix);
	queue RPN = {.maxsize = length, .tail = 0, .items = malloc(length * sizeof(thing))};
	stack opStack = {.maxsize = length, .top = 0, .items = malloc(length * sizeof(double))};
	double currNum = 0.00;
	char* toConvert; 
	toConvert = malloc(length * sizeof(char));
	toConvert[0] = '\0';
	int currentHighestPrecedance = -1;
	//Fill the queue with Null items
	for(int i = 0; i < length; i++){
		enqueue(&RPN, (thing){.type = 1, .value = 0});
	}
	thing temp = (thing){.type = 0, .value = 0};
	for(int i = 0; i < length; i++){
		if((infix[i] >= 48 && infix[i] <= 57) || infix[i] == 46 || infix[i] == '_'){
			strcharcat(toConvert, infix[i]);
		}
		else if((infix[i] == 'X') && MODE == ENTRY){
			if(strlen(toConvert) > 0){
				currNum = atod(toConvert);
				//Remove a null item from the queue
				dequeue(&RPN);
				//Add a real item to the queue
				enqueue(&RPN, (thing){.type = 0, .value = currNum});
			}
			dequeue(&RPN);
			enqueue(&RPN, (thing){.type = 2, .value = infix[i]});
		}
		else if((infix[i] >= 65) && (infix[i] <= 90)){
			//Handle a variable
			//Also handle constants (such as e or pi).
			if(strlen(toConvert) > 0){
				currNum = atod(toConvert);
				dequeue(&RPN);
				enqueue(&RPN, (thing){.type = 0, .value = currNum});
			}
			dequeue(&RPN);
			enqueue(&RPN, (thing){.type = 0, .value = vars[infix[i] - 65].value});
		}
		else if(infix[i] >= 97){
			push(&opStack, infix[i]);
		}
		else{
			//Handle an operation
			
			//Operations always come after a number, so clear the last number
			if(strlen(toConvert) > 0){
				currNum = atod(toConvert);
				dequeue(&RPN);
				enqueue(&RPN, (thing){.type = 0, .value = currNum});
				toConvert[0] = '\0';
			}
			//Handle low priority ops
			if(infix[i] == '+' || infix[i] == '-'){
				//Clear any equal or higher ops from the stack
				if(currentHighestPrecedance >= getPrecedence(infix[i]) && opStack.top > 0){
					while(getPrecedence(peek(&opStack)) >= getPrecedence(infix[i]) && opStack.top > 0 && getPrecedence(peek(&opStack)) < 10){
						temp = (thing){.type = 1, .value = pop(&opStack)};
						dequeue(&RPN);
						enqueue(&RPN, temp);
					}
				}
				//Add ourselves
				push(&opStack, infix[i]);
				currentHighestPrecedance = 0;
			}
			//Handle med priority ops
			else if(infix[i] == '*' || infix[i] == '/'){
				//Clear any equal or higher ops from the stack
				if(currentHighestPrecedance >= getPrecedence(infix[i])){
					while(getPrecedence(peek(&opStack)) >= getPrecedence(infix[i]) && opStack.top > 0 && getPrecedence(peek(&opStack))){
						temp = (thing){.type = 1, .value = pop(&opStack)};
						dequeue(&RPN);
						enqueue(&RPN, temp);
					}
				}
				//Add ourselves
				push(&opStack, infix[i]);
				currentHighestPrecedance = 1;
			}
			//Handle High priority ops
			else if(infix[i] == '^'){
				//Clear any equal ops from the stack (nothing is higher)
				if(currentHighestPrecedance >= getPrecedence(infix[i])){
					while(getPrecedence(peek(&opStack)) >= getPrecedence(infix[i]) && opStack.top > 0 && getPrecedence(peek(&opStack))){
						temp = (thing){.type = 1, .value = pop(&opStack)};
						dequeue(&RPN);
						enqueue(&RPN, temp);
					}
				}
				//Add ourselves
				push(&opStack, infix[i]);
				currentHighestPrecedance = getPrecedence(infix[i]);
			}
			//Handle an open paren
			else if(infix[i] == '('){
				//Just shove that fucker on there no questions asked.
				push(&opStack, infix[i]);
			}
			
			//Handle a close paren
			else if(infix[i] == ')'){
				//Just pop everything until we reach an open paren or a function (which are fancy open parens)
				while(getPrecedence(peek(&opStack)) < 10){
					temp = (thing){.type = 1, .value = pop(&opStack)};
					dequeue(&RPN);
					enqueue(&RPN, temp);
				}
				if(peek(&opStack) == '('){
					pop(&opStack);
				}
				else{
					temp = (thing){.type = 1, .value = pop(&opStack)};
					dequeue(&RPN);
					enqueue(&RPN, temp);
				}
			}
		}
	}
	//Queue the last number
	if(strlen(toConvert) > 0){
		currNum = atod(toConvert);
		dequeue(&RPN);
		enqueue(&RPN, (thing){.type = 0, .value = currNum});
	}
	//Clear the opstack
	while(opStack.top > 0){
		if(peek(&opStack) != '('){
			temp = (thing){.type = 1, .value = pop(&opStack)};
			dequeue(&RPN);
			enqueue(&RPN, temp);
		}
	}
	free(opStack.items, length * sizeof(double));
	free(toConvert, length * sizeof(char));
	return RPN;
}

double RPNResolve(queue RPN){
	stack solveStack = {.maxsize=255, .top=0, .items=0x00};
	solveStack.items = malloc(solveStack.maxsize * sizeof(double));
	double rand1, rand2;
	thing temp;
	int end = RPN.tail;
	for(int i = 0; i < end; i++){
		temp = dequeue(&RPN);
		if(temp.type == 0){
			push(&solveStack, temp.value);
		}
		else if(temp.type == 1){
			//Run the operation on the previous N items in the stack, where N is the number of arguments the op takes.
			//Not entirely sure how to do this in C. In scheme this would be easy-ish
			//Possibly a if-else tree
			if(temp.value == '+'){
				rand1 = pop(&solveStack);
				rand2 = pop(&solveStack);
				push(&solveStack, rand2 + rand1);
			}
			else if(temp.value == '-'){
				rand1 = pop(&solveStack);
				rand2 = pop(&solveStack);
				push(&solveStack, rand2 - rand1);
			}
			else if(temp.value == '*'){
				rand1 = pop(&solveStack);
				rand2 = pop(&solveStack);
				push(&solveStack, rand2 * rand1);
			}
			else if(temp.value == '/'){
				rand1 = pop(&solveStack);
				rand2 = pop(&solveStack);
				push(&solveStack, rand2 / rand1);
			}
			else if(temp.value == 's'){
				rand1 = pop(&solveStack);
				push(&solveStack, trigSin(rand1));
			}
			else if(temp.value == 'c'){
				rand1 = pop(&solveStack);
				push(&solveStack, trigCos(rand1));
			}
			else if(temp.value == 't'){
				rand1 = pop(&solveStack);
				push(&solveStack, trigTan(rand1));
			}
			else if(temp.value == 'a'){
				rand1 = pop(&solveStack);
				push(&solveStack, trigTan(rand1));
			}
			else if(temp.value == 'b'){
				rand1 = pop(&solveStack);
				push(&solveStack, trigTan(rand1));
			}
			else if(temp.value == 'd'){
				rand1 = pop(&solveStack);
				push(&solveStack, trigTan(rand1));
			}
			else if(temp.value == 'n'){
				rand1 = pop(&solveStack);
				push(&solveStack, trigTan(rand1));
			}
			else if(temp.value == 'g'){
				rand1 = pop(&solveStack);
				push(&solveStack, trigTan(rand1));
			}
		}
	}
	double result = pop(&solveStack);
	free(solveStack.items, solveStack.maxsize * sizeof(double));
	return result;
}

double pop(stack* stk){
	double ret = stk->items[stk->top];
	stk->top--;
	return ret;
}


//Note that Data can never be put at slot 0 of the stack.
int push(stack* stk, double item){
	if(stk->top + 1 < stk->maxsize){
		stk->top++;
		stk->items[stk->top] = item;
		return 0;
	}
	else{
		return -1;
	}
}

double peek(stack* stk){
	double ret = stk->items[stk->top];
	return ret;
}

thing dequeue(queue* q){
	thing stuff = q->items[0];
	for(int i = 1; i < q->tail; i++){
		q->items[i - 1] = q->items[i];
	}
	q->tail--;
	return stuff;
}

thing peekqueue(queue* q){
	return q->items[0];
}

void enqueue(queue* q, thing item){
	q->items[q->tail] = item;
	q->tail++;
}

void menuEntry(char* entry){
	if(strcmp(entry, "ONE")){
		
	}
	else if(strcmp(entry, "TWO")){
		
	}
	else{
		return;
	}
}


//We can have at most 24-4, or 20 entries on screen at a time.
//Based on that we can scroll based on entry being larger than 20
//Each entry will have a space before it, except for the current entry, which will have a star.
void showEntries(menu* men, int currTab, int entry){
	int top = 20 *(entry/20);
	int bottom = (men->tabs[currTab].numEntries < top + 20) ? men->tabs[currTab].numEntries : top+20;
	//Return character cursor to origin
	FBCharReset();
	//Go down 4 lines
	FBCharNewline();FBCharNewline();FBCharNewline();FBCharNewline();
	for(int i = top; i < bottom; i++){
		//Write each entry
		if(i == entry){
			FBWriteChar('*');
			FBWriteString(men->tabs[currTab].entries[i], strlen(men->tabs[currTab].entries[i]));
		}
		else{
			FBWriteChar(' ');
			FBWriteString(men->tabs[currTab].entries[i], strlen(men->tabs[currTab].entries[i]));
		}
		FBCharNewline();
	}
}


//We will take a step size, and continue on the step size until we have gone beyond the bounds of the screen.
//240/2 = 120, Y origin
//320/2 = 160, X origin
//Draw the axes first.
//Determine the scale by which Y increases or decreases by a pixel
//Then increment with that step size until we go off screen
//When we do, still continue calculating until we reach the X bounds
//Just don't draw whatever goes off

//Nevermind, we're interpolating
//When we get our next Y, we go over by 1 pixel in X, and fill in pixels of Y until we reach the pixel we calculated for Y.
//i.e. if at X=1 we got y=1, and for x=2 we got y=10, we would iterate from 2 to 10 (or, prevY + 1 to currY).
void graph(queue func, int xSize, int ySize){
	int i;
	double temp;
	//Draw axes in the center
	for(i = 0; i < maxWidth; i++){
		FBWrite(maxHeight/2, i, 0xFF);
	}
	for(i = 0; i < maxHeight; i++){
		FBWrite(i, maxWidth/2, 0xFF);
	}
	
	//X from -size/2 to size/2
	//Aka x from 0 to size, but minus size/2.
	//Increment x by xScale at each step
	int prevY, currY;
	for(int x = 0; x < 320; x++){
		currY = graphToPixelY(funcEval(func, pixelToGraphX(x, xSize)), ySize);
		if(currY != NaN){
			if(currY >= 0 && currY < maxWidth){
				if(x > 0){
					temp = -(prevY - currY);
					if(temp > 0){
						for(i = 1; i < temp; i++){
							FBWrite(x, prevY + i, 0xFF);
						}
					}
					else if(temp < 0){
						for(i = temp; i > 0; i--){
							FBWrite(x, prevY - i, 0xFF);
						}
					}
				}
				FBWrite(x, currY, 0xFF);
			}
			prevY = currY;
		}
	}
}

double pixelToGraphX(int x, int xSize){
	double scale = xSize/(maxHeight-1);
	return (x * scale) - (xSize/2);
}

double pixelToGraphY(int y, int ySize){
	double scale =ySize/(maxWidth-1);
	return (y * scale) - (ySize/2);
}

int graphToPixelX(double x, int xSize){
	double scale = (maxHeight-1)/xSize;
	return (x + (xSize/2)) * scale;
}

int graphToPixelY(double y, int ySize){
	double scale = (maxWidth-1)/ySize;
	return (y + (ySize/2)) * scale;
}

double funcEval(queue RPN, double x){
	stack solveStack = {.maxsize=255, .top=0, .items=0x00};
	solveStack.items = malloc(solveStack.maxsize * sizeof(double));
	double rand1, rand2;
	thing temp;
	for(int i = 0; i < RPN.tail; i++){
		temp = dequeue(&RPN);
		if(temp.type == 0){
			push(&solveStack, temp.value);
		}
		else if(temp.type == 1){
			//Run the operation on the previous N items in the stack, where N is the number of arguments the op takes.
			//Not entirely sure how to do this in C. In scheme this would be easy-ish
			//Possibly a if-else tree
			if(temp.value == '+'){
				rand1 = pop(&solveStack);
				rand2 = pop(&solveStack);
				push(&solveStack, rand2 + rand1);
			}
			else if(temp.value == '-'){
				rand1 = pop(&solveStack);
				rand2 = pop(&solveStack);
				push(&solveStack, rand2 - rand1);
			}
			else if(temp.value == '*'){
				rand1 = pop(&solveStack);
				rand2 = pop(&solveStack);
				push(&solveStack, rand2 * rand1);
			}
			else if(temp.value == '/'){
				rand1 = pop(&solveStack);
				rand2 = pop(&solveStack);
				push(&solveStack, rand2 / rand1);
			}
			else if(temp.value == 's'){
				rand1 = pop(&solveStack);
				push(&solveStack, trigSin(rand1));
			}
			else if(temp.value == 'c'){
				rand1 = pop(&solveStack);
				push(&solveStack, trigCos(rand1));
			}
			else if(temp.value == 't'){
				rand1 = pop(&solveStack);
				push(&solveStack, trigTan(rand1));
			}
			else if(temp.value == 'a'){
				rand1 = pop(&solveStack);
				push(&solveStack, trigTan(rand1));
			}
			else if(temp.value == 'b'){
				rand1 = pop(&solveStack);
				push(&solveStack, trigTan(rand1));
			}
			else if(temp.value == 'd'){
				rand1 = pop(&solveStack);
				push(&solveStack, trigTan(rand1));
			}
			else if(temp.value == 'n'){
				rand1 = pop(&solveStack);
				push(&solveStack, trigTan(rand1));
			}
			else if(temp.value == 'g'){
				rand1 = pop(&solveStack);
				push(&solveStack, trigTan(rand1));
			}
		}
		else if(temp.type == 2){
			push(&solveStack, x);
		}
		//Return the item to the end of the queue
		enqueue(&RPN, temp);
	}
	for(int i = 0; i < RPN.maxsize; i++){
		temp = peekqueue(&RPN);
		if(temp.type == 1 && temp.type == 0){
			temp = dequeue(&RPN);
			enqueue(temp);
			continue;
		}
		else{
			break;
		}
	}
	double result = pop(&solveStack);
	free(solveStack.items, solveStack.maxsize * sizeof(double));
	return result;
}

int KVGetValByKey(KVPair* dict, char search){
	for(int i = 0; i < 26; i++){
		if(dict[i].key == search){
			return dict[i].value;
		}
	}
	return -0;
}

void setList(int list, int item, double val){
	lists[list][item] = (thing){.type = 1, .value = val};
}

