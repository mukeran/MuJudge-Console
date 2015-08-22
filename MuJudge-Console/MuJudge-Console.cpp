/*
	@package MuJudge-Console
	@version 1.0
	@author mukeran
	@link http://www.mukeran.com
*/

#include "stdafx.h"
#include "MuJudge-Console.h"

#pragma warning(disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#define printfDEBUG(format, ...) printf("[MuJudge-Console][DEBUG][__TIME__] "format" on line: __LINE__ file: __FILE__ in funtion: __FUNCTION__", ##__VA_ARGS__);
#else
#define printfDEBUG(format, ...)
#endif

#pragma region define

#define DEFAULT_COLOR COLOR_WHITE

#define VERSION "1.2"

#define printferror(format, ...) setFontColor(COLOR_RED);printf(format, ##__VA_ARGS__);setFontColor(COLOR_WHITE);
#define printfcolor(color, format, ...) setFontColor(color);printf(format, ##__VA_ARGS__);setFontColor(DEFAULT_COLOR);

#define COLOR_ORGINAL 0
#define COLOR_WHITE 1
#define COLOR_RED 2
#define COLOR_GREEN 3
#define COLOR_BLUE 4
#define COLOR_YELLOW 5
#define COLOR_PINK 6
#define COLOR_CYAN 7

#define MAXN MAX_PATH

#pragma endregion

using namespace std;

#pragma region functions
void setFontColor(int i) {
	int colors[8] = {
		{ FOREGROUND_INTENSITY }, // i==0 为原色
		{ FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE }, //i==1 为白色
		{ FOREGROUND_INTENSITY | FOREGROUND_RED }, //i==2 为红色
		{ FOREGROUND_INTENSITY | FOREGROUND_GREEN }, //i==3 为绿色
		{ FOREGROUND_INTENSITY | FOREGROUND_BLUE }, //i==4 为蓝色
		{ FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN }, //i==5 为黄色
		{ FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE }, //i==6 为粉色
		{ FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE } //i==7 为青色
	};
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colors[i]);
}
void setBgColor(int i) {
	int colors[8] = {
		{ BACKGROUND_INTENSITY }, // i==0 为原色
		{ BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE }, //i==1 为白色
		{ BACKGROUND_INTENSITY | BACKGROUND_RED }, //i==2 为红色
		{ BACKGROUND_INTENSITY | BACKGROUND_GREEN }, //i==3 为绿色
		{ BACKGROUND_INTENSITY | BACKGROUND_BLUE }, //i==4 为蓝色
		{ BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN }, //i==5 为黄色
		{ BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE }, //i==6 为粉色
		{ BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE } //i==7 为青色
	};
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colors[i]);
}
void pressAnyButtonTo(char* test) {
	printf("请按任意键");
	printf(test);
	fflush(stdin);
	_getch();
}
void getCString(char* s) {
	memset(s, 0, sizeof s);
	fflush(stdin);
	char l;
	int len = 0;
	while ((l = getchar()) && (l != EOF) && (l != '\n')) {
		s[len++] = l;
	}
	s[len] = 0;
	len--;
	while (s[len] == '\n' || s[len] == ' ') {
		s[len] = 0;
		len--;
	}
	fflush(stdin);
}
void substr(char *dest, char *source, int start, int len) {
	for (int i = start; i < start + len; i++)
		dest[i - start] = source[i];
}
void clrscr() {
	system("cls");
}
void printTitle(int color = DEFAULT_COLOR) {
	printfcolor(color, "欢迎使用 MuJudge-Console !\n");
	printfcolor(color, "版本: %s\n\n", VERSION);
}
void noticeandexit(int exitcode) {
	fcloseall();
	DeleteFile(".\\tmp\\program.exe");

	if (_rmdir("tmp") == -1) {
		printf("删除临时目录 tmp 失败！请手动删除！\n");
	}
	pressAnyButtonTo("退出");
	exit(exitcode);
}
void writeToFile(const char *path, const char *content, bool rewrite) {
	FILE *fp = fopen(path, "r");
	if (fp != NULL) {
		fclose(fp);
		if (!rewrite) {
			return;
		}
	}
	fp = fopen(path, "w");
	fprintf(fp, content);
	fclose(fp);
}
bool CheckFolderExist(const string &strPath)
{
	WIN32_FIND_DATA  wfd;
	bool rValue = false;
	HANDLE hFind = FindFirstFile(strPath.c_str(), &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		rValue = true;
	}
	FindClose(hFind);
	return rValue;
}
#pragma endregion
int main(int argc, char* argv[], char* envp[]) {
#pragma region consolemode
	if (argc > 1) {
		printf("请不带任何参数运行程序，参数Judge还未开发出来！\n");
		if (strcmp(argv[1], "-help") == 0 || strcmp(argv[1], "-?") == 0) {
			printf("developing...\n");
		}
		exit(0);
	}
#pragma endregion
#pragma region programmode
	else {
	#pragma region begin
		SetConsoleTitle("MuJudge - Console");
		SetErrorMode(SEM_NOGPFAULTERRORBOX);
		printTitle();
	#pragma endregion
	#pragma region warning
		setFontColor(COLOR_PINK);
		printf("注意事项：\n");
		printf("1. 初版本不支持命令方式 Judge\n");
		printf("2. 本程序支持选择规则进行评测\n");
		printf("3. 以后版本会支持exe直接评测\n");
		printf("4. 时间测试可能不准确\n");
		setFontColor(COLOR_WHITE);
	#pragma endregion
	#pragma region config
		/*
		配置部分
		*/
		char pathofgcc[MAXN] = { 0 }, formatofin[MAXN] = { 0 }, formatofout[MAXN] = { 0 }, tmpmsecond[MAXN] = { 0 }, tmpjudgerules[MAXN] = { 0 }, tmpmemorylimit[MAXN] = { 0 };
		char pathofgccinput[MAXN] = { 0 };
		bool isconfig = false, tmpisconfig = false;

		#pragma region readautoconfig
		//检测读取配置
		FILE *fp = fopen("MuJudge-Console_Config.txt", "r");
		if (fp != NULL) {
			printfcolor(COLOR_CYAN, "\n找到基础配置文件，是否读取配置，回车即为是\n");
			while (true) {
				setFontColor(COLOR_WHITE);
				printf("readconfig[y/n]: ");
				fflush(stdin);
				char c = getchar();
				fflush(stdin);

				setFontColor(COLOR_RED);
				if (c == 'Y' || c == 'y' || c == '\n') {
					isconfig = true;
				}
				else if (c == 'N' || c == 'n') {
					isconfig = false;
				}
				else {
					printf("输入错误，请检查后重新输入\n");
					continue;
				}
				break;
			}
			if (isconfig) {
				char tmpin[MAXN];
				while (true) {
					memset(tmpin, 0, sizeof tmpin);
					if (fgets(tmpin, MAXN, fp) == NULL) {
						break;
					}
					int len = strlen(tmpin) - 1;
					while (tmpin[len] == '\n' || tmpin[len] == '\r' || tmpin[len] == ' ') {
						tmpin[len] = 0;
						len--;
					}
					string tmpstr = tmpin;
					if (tmpstr.substr(0, 5) == "gcc: ") {
						memset(pathofgcc, 0, sizeof pathofgcc);
						strcpy(pathofgcc, tmpstr.substr(5, tmpstr.length() - 5).c_str());
					}
					else if (tmpstr.substr(0, 4) == "gcc:") {
						memset(pathofgcc, 0, sizeof pathofgcc);
						strcpy(pathofgcc, tmpstr.substr(4, tmpstr.length() - 4).c_str());
					}
					else if (tmpstr.substr(0, 10) == "fileofin: ") {
						memset(formatofin, 0, sizeof formatofin);
						strcpy(formatofin, tmpstr.substr(10, tmpstr.length() - 10).c_str());
					}
					else if (tmpstr.substr(0, 9) == "fileofin:") {
						memset(formatofin, 0, sizeof formatofin);
						strcpy(formatofin, tmpstr.substr(9, tmpstr.length() - 9).c_str());
					}
					else if (tmpstr.substr(0, 11) == "fileofout: ") {
						memset(formatofout, 0, sizeof formatofout);
						strcpy(formatofout, tmpstr.substr(11, tmpstr.length() - 11).c_str());
					}
					else if (tmpstr.substr(0, 10) == "fileofout:") {
						memset(formatofout, 0, sizeof formatofout);
						strcpy(formatofout, tmpstr.substr(10, tmpstr.length() - 10).c_str());
					}
					else if (tmpstr.substr(0, 9) == "timeout: ") {
						memset(tmpmsecond, 0, sizeof tmpmsecond);
						strcpy(tmpmsecond, tmpstr.substr(9, tmpstr.length() - 9).c_str());
					}
					else if (tmpstr.substr(0, 8) == "timeout:") {
						memset(tmpmsecond, 0, sizeof tmpmsecond);
						strcpy(tmpmsecond, tmpstr.substr(8, tmpstr.length() - 8).c_str());
					}
					else if (tmpstr.substr(0, 13) == "memorylimit: ") {
						memset(tmpmsecond, 0, sizeof tmpmemorylimit);
						strcpy(tmpmemorylimit, tmpstr.substr(13, tmpstr.length() - 13).c_str());
					}
					else if (tmpstr.substr(0, 12) == "memorylimit:") {
						memset(tmpmsecond, 0, sizeof tmpmemorylimit);
						strcpy(tmpmemorylimit, tmpstr.substr(12, tmpstr.length() - 12).c_str());
					}
					else if (tmpstr.substr(0, 12) == "judgerules: ") {
						memset(tmpjudgerules, 0, sizeof tmpjudgerules);
						strcpy(tmpjudgerules, tmpstr.substr(12, tmpstr.length() - 12).c_str());
					}
					else if (tmpstr.substr(0, 11) == "judgerules:") {
						memset(tmpjudgerules, 0, sizeof tmpjudgerules);
						strcpy(tmpjudgerules, tmpstr.substr(11, tmpstr.length() - 11).c_str());
					}
				}
			}
			fclose(fp);
		}
		#pragma endregion
		#pragma region config_complier
		//编译器设置与检测
		printfcolor(COLOR_CYAN, "\n请输入MinGW下的bin路径，像D:\\example\\bin这样。如果设置了MinGW环境变量或者其它类型gcc编译器，MinGW环境变量一定要为*\\MinGW\\bin，请直接回车\n");
		tmpisconfig = isconfig;
		bool ispath = false;
		while (true) {
			memset(pathofgccinput, 0, sizeof pathofgccinput);
			setFontColor(COLOR_WHITE);
			printf("gcc: ");
			if (tmpisconfig) {
				printf(pathofgcc);
				printf("\n");
				tmpisconfig = false;
			}
			else {
				getCString(pathofgcc);
			}

			if (strcmp(pathofgcc, "forcepath") == 0) {
				memset(pathofgcc, 0, sizeof pathofgcc);
				strcpy(pathofgcc, "gcc");
				strcpy(pathofgccinput, "forcepath");
				break;
			}
			setFontColor(COLOR_RED);
			if (pathofgcc[0] == 0) { //设置了环境变量
				ispath = true;
				char *path;
				path = getenv("path");
				char *isok = strstr(path, "MinGW\\bin");
				if (isok == NULL) {
					printf("没有在环境变量中找到MinGW的路径，是否要强制使用path进行评测，回车即为否\n");
					bool isforce = false;
					while (true) {
						printf("force to user path[y/n]: ");

						fflush(stdin);
						char tmpc = getchar();
						fflush(stdin);
						if (tolower(tmpc) == 'y') {
							isforce = true;
							break;
						}
						else if (tolower(tmpc) == 'n' || tmpc == '\n') {
							break;
						}
					}
					if (!isforce) {
						printf("请换用路径或者检查环境变量后重新输入\n");
						continue;
					}
					else {
						memset(pathofgcc, 0, sizeof pathofgcc);
						strcpy(pathofgcc, "gcc");
						strcpy(pathofgccinput, "forcepath");
						break;
					}
				}
				else {
					int pos = isok - path;
					int pathlen = strlen(path);
					int leftpos = 0, rightpos = pathlen - 1;
					for (int i = pos - 1; i >= 0; i--)
						if (path[i] == ';') {
							leftpos = i + 1;
							break;
						}
					for (int i = pos + 1; i < pathlen; i++)
						if (path[i] == ';') {
							rightpos = i - 1;
							break;
						}
					char mingwpath[MAXN];
					memset(mingwpath, 0, sizeof mingwpath);
					int mingwpathlen = rightpos - leftpos + 1;
					substr(mingwpath, path, leftpos, mingwpathlen);
					if (mingwpath[mingwpathlen - 1] == 'n')
						strcat(mingwpath, "\\");
					strcpy(pathofgccinput, mingwpath);
					strcat(mingwpath, "gcc.exe");
					strcpy(pathofgcc, mingwpath);
				}
			}
			else {
				strcpy(pathofgccinput, pathofgcc);
				string filename = pathofgcc;
				filename += "\\gcc.exe";
				memset(pathofgcc, 0, sizeof pathofgcc);
				strcpy(pathofgcc, filename.c_str());
			}

			FILE* fp = fopen(pathofgcc, "r");
			if (fp == NULL) {
				printf("未找到gcc.exe或者路径不合法，请检查后重新输入\n");
				continue;
			}
			fclose(fp);
			break;
		}
		#pragma endregion
		#pragma region config_inputfile
		//输入文件检查
		printfcolor(COLOR_CYAN, "\n请输入输入文件的文件名或路径，有后缀名一定要包含后缀名，用[0]来表示数字递增从0开始，用[00]来表示数字递增从00开始，用[1-100]来表示数字只在1和100之间，首个输入文件必须存在\n");
		char inputmainfilename[MAXN], inputmainfilesuffix[MAXN];
		int startnum = 0, endnum = 0, startnumlen = 0, endnumlen = 0, tmpinputlen = 0;
		bool haslimit = false;
		tmpisconfig = isconfig;
		while (true) {
			setFontColor(COLOR_WHITE);
			printf("fileofin: ");
			if (tmpisconfig) {
				printf(formatofin);
				printf("\n");
				tmpisconfig = false;
			}
			else {
				getCString(formatofin);
			}

			setFontColor(COLOR_RED);
			int formatofinlen = strlen(formatofin);
			startnum = 0, endnum = 0, startnumlen = 0, endnumlen = 0, tmpinputlen = 0;
			bool isright = false;
			memset(inputmainfilename, 0, sizeof inputmainfilename);
			//读取[]内容
			int i, j;
			for (i = 0; i < formatofinlen; i++) {
				if (formatofin[i] == '[') {
					for (j = i + 1; j < formatofinlen; j++) {
						if (formatofin[j] == ']') {
							if (j - i == 1) {
								isright = false;
							}
							else {
								isright = true;
							}
							break;
						}
						else if (formatofin[j] == '-') {
							if (!haslimit) {
								haslimit = true;
							}
							else {
								break;
							}
						}
						else {
							if (isdigit(formatofin[j])) {
								if (!haslimit) {
									startnum = startnum * 10 + formatofin[j] - '0';
									startnumlen++;
								}
								else {
									endnum = endnum * 10 + formatofin[j] - '0';
									endnumlen++;
								}
							}
							else if (formatofin[j] != '-'&&formatofin[j] != ']')
								break;
						}
					}
					break;
				}
				else inputmainfilename[tmpinputlen++] = formatofin[i];
			}
			if (!isright) {
				printf("你输入的格式有误，请检查后重新输入\n");
				continue;
			}
			int error = NULL;
			tmpinputlen = 0;
			//判断后面的内容是否合法，以及获取后面的内容
			memset(inputmainfilesuffix, 0, sizeof inputmainfilesuffix);
			for (i = j + 1; i < formatofinlen; i++) {
				if (formatofin[i] == '\\') {
					error = 1;
					break;
				}
				else if (formatofin[i] == '[' || formatofin[i] == ']') {
					error = 2;
					break;
				}
				else
					inputmainfilesuffix[tmpinputlen++] = formatofin[i];
			}
			if (error == 1) {
				printf("目录中不允许有[]，请检查后重新输入\n");
				continue;
			}
			if (error == 2){
				printf("不允许有两处[]，请检查后重新输入\n");
				continue;
			}
			else {
				if (startnumlen != endnumlen && startnumlen != 1 && endnum != 0) {
					printf("起始数字的长度和终点数字的长度不同，在后面的判定会出现故障，请检查后重新输入\n");
					continue;
				}
				else {
					//验证首个输入文件是否存在
					char firstinputfilepath[MAXN];
					memset(firstinputfilepath, 0, sizeof firstinputfilepath);
					strcat(firstinputfilepath, inputmainfilename);
					char format[MAXN] = "%0";
					char tmp[MAXN];
					memset(tmp, 0, sizeof tmp);
					itoa(startnumlen, tmp, 10);
					strcat(format, tmp);
					strcat(format, "d");
					memset(tmp, 0, sizeof tmp);
					sprintf(tmp, format, startnum);
					strcat(firstinputfilepath, tmp);
					strcat(firstinputfilepath, inputmainfilesuffix);

					FILE *fp = fopen(firstinputfilepath, "r");
					if (fp == NULL) {
						printf("首个输入文件不存在，请检查后重新输入\n");
						continue;
					}
					fclose(fp);
				}
			}
			break;
		}
		#pragma endregion
		#pragma region config_outputfile
		//输出文件存在性检测
		printfcolor(COLOR_CYAN, "\n请输入输出文件的文件名或路径，输出文件的数字必须与输入文件一一对应，所以数字部分请使用[]代替\n");
		char outputmainfilename[MAXN], outputmainfilesuffix[MAXN];
		tmpisconfig = isconfig;
		while (true) {
			setFontColor(COLOR_WHITE);
			printf("fileofout: ");
			if (tmpisconfig) {
				printf(formatofout);
				printf("\n");
				tmpisconfig = false;
			}
			else {
				getCString(formatofout);
			}

			setFontColor(COLOR_RED);
			int formatofoutlen = strlen(formatofout);

			memset(outputmainfilename, 0, sizeof outputmainfilename);
			int i, tmpoutputlen = 0;
			bool isright = false;
			for (i = 0; i < formatofoutlen - 1; i++) {
				if (formatofout[i] == '['&&formatofout[i + 1] == ']') {
					isright = true;
					break;
				}
				else outputmainfilename[tmpoutputlen++] = formatofout[i];
			}
			if (!isright) {
				printf("你输入的格式有误，请检查后重新输入\n");
				continue;
			}
			int error = NULL;
			tmpoutputlen = 0;
			//判断后面的内容是否合法，以及获取后面的内容
			memset(outputmainfilesuffix, 0, sizeof outputmainfilesuffix);
			for (i += 2; i < formatofoutlen; i++) {
				if (formatofout[i] == '\\') {
					error = 1;
					break;
				}
				else if (formatofout[i] == '[' || formatofout[i] == ']') {
					error = 2;
					break;
				}
				else outputmainfilesuffix[tmpoutputlen++] = formatofout[i];
			}
			if (error == 1) {
				printf("目录中不允许有[]，请检查后重新输入\n");
				continue;
			}
			if (error == 2){
				printf("不允许有两处[]，请检查后重新输入\n");
				continue;
			}
			else {
				//验证首个输出文件是否存在
				char firstoutputfilepath[MAXN];
				memset(firstoutputfilepath, 0, sizeof firstoutputfilepath);
				strcat(firstoutputfilepath, outputmainfilename);
				char format[MAXN] = "%0";
				char tmp[MAXN];
				memset(tmp, 0, sizeof tmp);
				itoa(startnumlen, tmp, 10);
				strcat(format, tmp);
				strcat(format, "d");
				memset(tmp, 0, sizeof tmp);
				sprintf(tmp, format, startnum);
				strcat(firstoutputfilepath, tmp);
				strcat(firstoutputfilepath, outputmainfilesuffix);

				FILE *fp = fopen(firstoutputfilepath, "r");
				if (fp == NULL) {
					printf("首个输出文件不存在，请检查后重新输入\n");
					continue;
				}
				fclose(fp);
			}
			break;
		}
		#pragma endregion
		#pragma region config_timeout
		//超时设置
		printfcolor(COLOR_CYAN, "\n请输入程序超时时间，以毫秒(ms)为单位，必须为整数，直接回车或者输入 0 则为不限时间\n");
		int msecond = INFINITE;
		tmpisconfig = isconfig;
		while (true) {
			setFontColor(COLOR_WHITE);
			printf("timeout: ");
			if (tmpisconfig) {
				printf(tmpmsecond);
				printf("\n");
				tmpisconfig = false;
			}
			else {
				getCString(tmpmsecond);
			}

			setFontColor(COLOR_RED);
			if (tmpmsecond[0] == 0 || strcmp(tmpmsecond, "infinite") == 0 || strcmp(tmpmsecond, "INFINITE") == 0) {
				break;
			}

			int tmpmsecondlen = strlen(tmpmsecond);
			msecond = 0;
			int error = NULL;
			for (int i = 0; i < tmpmsecondlen; i++) {
				if (tmpmsecond[i] == '.'&&i > 0 && isdigit(tmpmsecond[i - 1])) {
					error = 1;
					break;
				}
				if (!isdigit(tmpmsecond[i])) {
					error = 2;
					break;
				}
				msecond = msecond * 10 + tmpmsecond[i] - '0';
			}
			if (error == 1) {
				printf("输入的数字只能为整数，请检查后重新输入\n");
				continue;
			}
			else if (error == 2) {
				printf("你输入的不是数字，请检查后重新输入\n");
				continue;
			}
			if (msecond == 0) {
				msecond = INFINITE;
			}
			break;
		}
		#pragma endregion
		#pragma region config_memorylimit
		//内存设置默认128M
		printfcolor(COLOR_CYAN, "\n请输入最大内存限制，以兆字节(MB)为单位，如果要使用千字节(kB)作为单位，请在数字后面加上k，回车即为默认 128M\n");
		int memorylimit = NULL;
		tmpisconfig = isconfig;
		while (true) {
			setFontColor(COLOR_WHITE);
			printf("memorylimit: ");
			if (tmpisconfig) {
				printf(tmpmemorylimit);
				printf("\n");
				tmpisconfig = false;
			}
			else {
				getCString(tmpmemorylimit);
			}

			setFontColor(COLOR_RED);
			if (tmpmemorylimit[0] == 0) {
				memorylimit = 128 * 1024;
				break;
			}
			
			memorylimit = 0;
			int tmpmemorylimitlen = strlen(tmpmemorylimit);
			bool istrue = true, iskb = false;
			for (int i = 0; i < tmpmemorylimitlen; i++) {
				if (isdigit(tmpmemorylimit[i])) {
					memorylimit = memorylimit * 10 + tmpmemorylimit[i] - '0';
				}
				else if (tolower(tmpmemorylimit[i]) == 'k') {
					iskb = true;
					break;
				}
				else if (tolower(tmpmemorylimit[i]) == 'm') {
					break;
				}
				else {
					printf("输入错误，请重新输入！\n");
					istrue = false;
					break;
				}
			}
			if (!iskb) memorylimit *= 1024;
			if (!istrue) {
				continue;
			}
			break;
		}
		#pragma endregion
		#pragma region config_judgerules
		//评测模式设置
		printfcolor(COLOR_CYAN, "\n请输入数字序号来设置评测规则，多个规则请连写，如123，直接回车则100%相同则 AC，所有规则可重复，且有顺序处理，且同时处理标准输出和程序输出\n");
		printf("1. 忽略输出末尾的换行;\n");
		printf("2. 忽略每行前端的空格;\n");
		printf("3. 忽略每行末尾的空格;\n");
		printf("4. 忽略重复换行;\n");
		printf("5. 忽略所有空格;\n");
		printf("6. 忽略所有换行;\n");
		
		int judgerules[MAXN] = {0}, judgeruleslen = 0;
		tmpisconfig = isconfig;
		while (true) {
			setFontColor(COLOR_WHITE);
			printf("judgerules: ");

			if (tmpisconfig) {
				printf(tmpjudgerules);
				printf("\n");
				tmpisconfig = false;
			}
			else {
				getCString(tmpjudgerules);
			}

			int tmpjudgeruleslen = strlen(tmpjudgerules);
			for (int i = 0; i < tmpjudgeruleslen; i++) {
				if (tmpjudgerules[i] >= '1'&&tmpjudgerules[i] <= '6'){
					judgerules[i] = tmpjudgerules[i] - '0';
				}
				else {
					printf("你的输入有误，请检查后重新输入\n");
				}
			}
			judgeruleslen = tmpjudgeruleslen;
			break;
		}

		#pragma endregion
		#pragma region saveconfig
		//保存基础配置
		fp = fopen("MuJudge-Console_Config.txt", "w+");
		fprintf(fp, "gcc: %s\n", pathofgccinput);
		fprintf(fp, "fileofin: %s\n", formatofin);
		fprintf(fp, "fileofout: %s\n", formatofout);
		char msecondstrtmp[MAXN];
		itoa(msecond, msecondstrtmp, 10);
		fprintf(fp, "timeout: %s\n", msecond == INFINITE ? "infinite" : msecondstrtmp);
		fprintf(fp, "memorylimit: %d\n", memorylimit);
		fprintf(fp, "judgerules: %s\n", tmpjudgerules);
		fclose(fp);
		printf("\n基础配置保存成功！\n");
		#pragma endregion
		#pragma region config_source
		//源程序存在性检测
		printfcolor(COLOR_CYAN, "\n请输入源文件名或路径，源文件名应该是这样的：example.c或者example.cpp\n");
		char pathofsource[MAXN];
		while (true) {
			setFontColor(COLOR_WHITE);
			printf("source: ");
			getCString(pathofsource);

			setFontColor(COLOR_RED);
			int pathofsourcelen = strlen(pathofsource);

			//检查后缀是否为.c或者.cpp
			bool isright = false;
			if (pathofsourcelen > 4) {
				char suffix[10];
				memset(suffix, 0, sizeof suffix);
				substr(suffix, pathofsource, pathofsourcelen - 4, 4);
				if (strcmp(suffix, ".cpp") != 0) {
					memset(suffix, 0, sizeof suffix);
					substr(suffix, pathofsource, pathofsourcelen - 2, 2);
					if (strcmp(suffix, ".c") == 0) isright = true;
				}
				else isright = true;
			}
			else if (pathofsourcelen > 2) {
				char suffix[10];
				memset(suffix, 0, sizeof suffix);
				substr(suffix, pathofsource, pathofsourcelen - 2, 2);
				if (strcmp(suffix, ".c") == 0) isright = true;
			}
			if (!isright) {
				if (strcmp(pathofsource, ".c") == 0 || strcmp(pathofsource, ".cpp") == 0) {
					printf("你输入的源文件名有误，请检查后重新输入\n");
				}
				else {
					printf("你输入的源文件名或路径后缀不为.c或者.cpp，请检查后重新输入\n");
				}
				continue;
			}

			FILE *fp = fopen(pathofsource, "r");
			if (fp == NULL) {
				printf("源文件不存在，请检查后重新输入\n");
				continue;
			}
			else fclose(fp);
			break;
		}
		#pragma endregion
		#pragma region config_isfreopen
		//选择是否已经重定向加入标准输入输出流
		printfcolor(COLOR_CYAN, "\n请选择你是否已经在程序中加入重定向标准输入输出流的语句，如freopen，回车即为否\n");
		bool isfreopen = false;
		while (true) {
			setFontColor(COLOR_WHITE);
			printf("freopen or others[y/n]: ");

			fflush(stdin);
			char c = getchar();
			fflush(stdin);

			setFontColor(COLOR_RED);
			if (c == 'Y' || c == 'y') {
				isfreopen = true;
			}
			else if (c == 'N' || c == 'n' || c == '\n') {
				isfreopen = false;
			}
			else {
				printf("你输入的有误，请重新输入\n");
				continue;
			}
			break;
		}
		char freopenstdin[MAXN], freopenstdout[MAXN];
		if (isfreopen) {
			printfcolor(COLOR_CYAN, "\n请输入重定向标准输入的替代文件名（本句没有安全性检查）\n");
			while (true) {
				setFontColor(COLOR_WHITE);
				printf("freopen or others stdin: ");
				getCString(freopenstdin);

				setFontColor(COLOR_RED);
				if (freopenstdin[0] == 0){
					printf("你没有输入，请重新输入\n");
					continue;
				}
				break;
			}

			printfcolor(COLOR_CYAN, "\n请输入重定向标准输出的替代文件名（本句没有安全性检查）\n");
			while (true) {
				setFontColor(COLOR_WHITE);
				printf("freopen or others stdout: ");
				getCString(freopenstdout);

				setFontColor(COLOR_RED);
				if (freopenstdout[0] == 0){
					printf("你没有输入，请重新输入\n");
					continue;
				}
				break;
			}
		}
		#pragma endregion
		#pragma region showconfig
		clrscr();
		printTitle();
		setFontColor(COLOR_PINK);
		printf("你输入的信息：\n");
		printf("gcc: %s\n", pathofgccinput);
		printf("fileofin: %s\n", formatofin);
		printf("fileofout: %s\n", formatofout);
		memset(msecondstrtmp, 0, sizeof msecondstrtmp);
		itoa(msecond, msecondstrtmp, 10);
		printf("timeout: %s\n", msecond == INFINITE ? "infinite" : msecondstrtmp);
		printf("memorylimit: %d kB\n", memorylimit);
		printf("judgerules: %s\n", tmpjudgerules);
		printf("-----\n");
		printf("source: %s\n", pathofsource);
		printf("freopen or others[y/n]: %s\n", isfreopen ? "y" : "n");
		if (isfreopen) {
			printf("freopen or others stdin: \n", freopenstdin);
			printf("freopen or others stdout: \n", freopenstdout);
		}
		setFontColor(COLOR_WHITE);
		#pragma endregion
#pragma endregion
	#pragma region createtmpdir
		if (!CheckFolderExist("tmp")) {
			if (_mkdir("tmp") == -1) {
				printf("创建tmp目录失败，请手动创建一个！\n");
			}
		}
	#pragma endregion
	#pragma region complier
		/*
		编译部分
		*/
		printf("\n开始编译\n");
		#pragma region docomplie
		SECURITY_ATTRIBUTES sa;
		HANDLE hRead, hWrite;

		sa.nLength = sizeof SECURITY_ATTRIBUTES;
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;
		if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
			printferror("无法创建读取编译器返回信息的管道！\n");
			printferror("错误代码: %d\n", GetLastError());
			noticeandexit(1);
		}

		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof STARTUPINFO);
		si.cb = sizeof STARTUPINFO;
		GetStartupInfo(&si);
		si.hStdError = hWrite;
		si.hStdOutput = hWrite;
		si.wShowWindow = SW_HIDE;
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

		char cmdline[MAXN];
		memset(cmdline, 0, sizeof cmdline);
		sprintf(cmdline, "%s %s -o .\\tmp\\program.exe -fno-asm -O2 -Wall -lm --static", pathofgcc, pathofsource);

		bool f = true;
		int exitcode = 0;
		if (CreateProcess(NULL, cmdline, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi)) {
			int result = WaitForSingleObject(pi.hProcess, INFINITE);
			if (result == WAIT_OBJECT_0) {
				f = true;
				if (GetExitCodeProcess(pi.hProcess, (DWORD *)&exitcode)) {

				}
				else
				{
					printferror("错误！不能读取编译器的返回值！\n");
					noticeandexit(1);
				}
			}
			else {
				printferror("错误！\n");
				printferror("错误代码: %d\n", GetLastError());
				noticeandexit(1);
			}
		}
		else {
			if (strcmp(pathofgcc, "gcc")) {
				printferror("编译错误！请检查环境变量是否真正设置正确！\n");
			}
			else {
				printferror("编译错误！请检查编译器是否存在。\n");
			}
			printferror("错误代码: %d\n", GetLastError());
			noticeandexit(1);
		}
		CloseHandle(hWrite);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		#pragma endregion
		#pragma region counterrandwarning
		char buffer[1] = { 0 };
		string strOutput = "";
		DWORD bytesRead;
		int cnterr, cntwarn;
		cnterr = cntwarn = 0;
		string line = "";

		while (true) {
			if (ReadFile(hRead, buffer, 1, &bytesRead, NULL) == NULL) {
				break;
			}
			char l = buffer[0];
			if (l == '\r') continue;
			if (l == '\n') {
				if (line.find(": error:") != -1) {
					cnterr++;
				}
				else if (line.find(": warning:") != -1) {
					cntwarn++;
				}
				line = "";
			}
			else {
				line += buffer[0];
			}
			strOutput += buffer[0];
		}
		CloseHandle(hRead);
		#pragma endregion
		#pragma region outputinformation
		if (exitcode == 1) {
			if (cnterr == 0 && cntwarn == 0 && strOutput != "") {
				setFontColor(COLOR_RED);
				printf("编译错误！\n");
				setFontColor(COLOR_WHITE);
			}
			else {
				setFontColor(COLOR_RED);
				printf("编译结束，但存在 %d 个错误", cnterr);
				if (cntwarn != 0) {
					setFontColor(COLOR_YELLOW);
					printf(" %d 个警告", cntwarn);
				}
				printf("\n");
				setFontColor(COLOR_WHITE);
			}
			writeToFile("MuJudge-Console_Compilerlog.txt", strOutput.c_str(), true);
			printf("编译信息已经保存在当前目录MuJudge-Console_Compilerlog.txt中，请修正错误后再来Judge！\n");
			noticeandexit(1);
		}
		else if (exitcode == 0) {
			if (cntwarn == 0) {
				printf("编译成功！\n");
			}
			else {
				setFontColor(COLOR_YELLOW);
				printf("编译结束，但存在 %d 个警告\n", cntwarn);
				setFontColor(COLOR_WHITE);
				writeToFile("MuJudge-Console_Compilerlog.txt", strOutput.c_str(), true);
				printf("编译信息已经保存在当前目录MuJudge-Console_Compilerlog.txt中\n");
			}
		}
		printf("\n");
		#pragma endregion
	#pragma endregion
	#pragma region judge
		/*
		Judge部分
		*/
		pressAnyButtonTo("开始评测");
		printf("\n开始评测\n");
		printf("\n");
		int cnt = startnum;
		char format[MAXN] = "%0";
		char tmp[MAXN];
		memset(tmp, 0, sizeof tmp);
		itoa(startnumlen, tmp, 10);
		strcat(format, tmp);
		strcat(format, "d");
		int AC, WA, PE, TLE, RE, MLE;
		AC = WA = PE = TLE = RE = MLE = 0;
		FILE *outputrs = fopen("MuJudge-Console_JudgeResult.txt", "w+");
		fprintf(outputrs, "MuJudge-Console 评测结果\n\n");
		while (true) {
			#pragma region doprogram
			if (cnt > endnum && endnum != 0) {
				break;
			}

			string filein = "";
			filein.append(inputmainfilename);
			memset(tmp, 0, sizeof tmp);
			sprintf(tmp, format, cnt);
			filein.append(tmp);
			filein.append(inputmainfilesuffix);

			string fileout = "";
			fileout.append(outputmainfilename);
			memset(tmp, 0, sizeof tmp);
			sprintf(tmp, format, cnt);
			fileout.append(tmp);
			fileout.append(outputmainfilesuffix);

			FILE *checkin = fopen(filein.c_str(), "r");
			FILE *checkout = fopen(fileout.c_str(), "r");
			if (checkin == NULL && checkout == NULL) {
				if (endnum != 0) {
					printferror("编号 %d 输入输出文件 不存在！\n", cnt);
					fprintf(outputrs, "编号 %d | 文件 %s 和 %s 不存在！\n", cnt, filein.c_str(), fileout.c_str());
				}
			}
			else if (checkin == NULL) {
				printferror("编号 %d 输入文件 不存在！\n", cnt);
				fprintf(outputrs, "编号 %d | 文件 %s 不存在！\n", cnt, filein.c_str());
				fclose(checkout);
			}
			else if (checkout == NULL) {
				printferror("编号 %d 输出文件 不存在！\n", cnt);
				fprintf(outputrs, "编号 %d | 文件 %s 不存在！\n", cnt, fileout.c_str());
				fclose(checkin);
			}
			if (checkin == NULL || checkout == NULL) {
				if (endnum != 0) {
					cnt++;
					printf("\n");
					continue;
				}
				else {
					break;
				}
			}
			fclose(checkin);
			fclose(checkout);

			STARTUPINFO si;
			PROCESS_INFORMATION pi;
			HANDLE hIn = new HANDLE, hOutRead = new HANDLE, hOutWrite = new HANDLE;

			ZeroMemory(&si, sizeof STARTUPINFO);
			GetStartupInfo(&si);
			si.cb = sizeof STARTUPINFO;

			string targetinput = ".\\tmp\\", targetoutput = ".\\tmp\\";
			if (!isfreopen) {
				SECURITY_ATTRIBUTES sa;

				sa.nLength = sizeof SECURITY_ATTRIBUTES;
				sa.lpSecurityDescriptor = NULL;
				sa.bInheritHandle = TRUE;

				hIn = CreateFile(filein.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if (!CreatePipe(&hOutRead, &hOutWrite, &sa, 0)) {
					printferror("创建输入管道失败！\n");
					noticeandexit(1);
				}
				si.hStdInput = hIn;
				si.hStdError = hOutWrite;
				si.hStdOutput = hOutWrite;
			}
			else {
				targetinput += freopenstdin;
				int returncode = CopyFile(filein.c_str(), targetinput.c_str(), false);
				if (returncode == 0) {
					printferror("复制输入文件时出错！\n");
					printferror("错误代码: %d\n", GetLastError());
					noticeandexit(1);
				}
			}

			if (!isfreopen) {
				si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
			}
			else {
				si.dwFlags = STARTF_USESHOWWINDOW;
			}
			si.wShowWindow = SW_HIDE;

			int rs = NULL;
			int ms, kb;
			int exitcode = 0;
			if (CreateProcess(".\\tmp\\program.exe", NULL, NULL, NULL, !isfreopen, CREATE_SUSPENDED, NULL, ".\\tmp", &si, &pi)) {
				ResumeThread(pi.hThread);
				clock_t start = clock();
				int result = WaitForSingleObject(pi.hProcess, msecond);
				clock_t finish = clock();
				PROCESS_MEMORY_COUNTERS pmc;
				GetProcessMemoryInfo(pi.hProcess, &pmc, sizeof pmc);
				kb = pmc.PeakPagefileUsage / 1024;
				ms = (int)(((double)(finish - start) / CLOCKS_PER_SEC) * 1000);
				if (result == WAIT_OBJECT_0) {
					rs = 1;
					if (kb > memorylimit) rs = 4;
				}
				else if (result == WAIT_TIMEOUT) {
					rs = 2;
				}
				else {
					printferror("Judge程序错误！\n");
					printferror("错误代码: %d\n", GetLastError());
					noticeandexit(1);
				}
				if (GetExitCodeProcess(pi.hProcess, (DWORD *)&exitcode)) {
					if (exitcode != 0 && rs != 2) {
						rs = 3;
					}
				}
				else
				{
					printferror("错误！不能读取被评测程序的返回值！\n");
					printferror("错误代码: %d\n", GetLastError());
					noticeandexit(1);
				}
			}
			else {
				printferror("无法创建被评测程序进程，请检查 tmp 目录下被评测程序是否存在！\n");
				noticeandexit(1);
			}
			if (!isfreopen) {
				CloseHandle(hIn);
				CloseHandle(hOutWrite);
			}
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
			if (rs == 2) {
				if (!TerminateProcess(pi.hThread, 0)) {
					int code = GetLastError();
					if (code != 6) {
						printferror("无法停止运行子程序线程，Judge强制停止\n");
						printferror("请自行关闭窗口结束进程\n");
						printferror("错误代码: %d\n", code);
						noticeandexit(1);
					}
				}
				if (!TerminateProcess(pi.hProcess, 0)) {
					int code = GetLastError();
					if (code != 6) {
						printferror("无法停止运行子程序进程，Judge强制停止\n");
						printferror("请自行关闭窗口结束进程\n");
						printferror("错误代码: %d\n", code);
						noticeandexit(1);
					}
				}
			}
			#pragma endregion
			if (rs == 1) {
			#pragma region readoutput
				char buffer[4096] = { 0 };
				DWORD bytesRead;
				string programOutput = "";
				if (isfreopen) { //如果是重定向了输入输出，则寻找输出的文件
					targetoutput += freopenstdout;
					FILE *fo = fopen(targetoutput.c_str(), "r");
					if (fo == NULL) {
						printferror("没有找到被评测程序输出文件！\n");
						noticeandexit(1);
					}
					char s1;
					while ((s1 = fgetc(fo)) && (s1 != EOF)) {
						programOutput += s1;
					}
					fclose(fo);
				}
				else { //如果不是重定向，则从管道中读取
					while (true) {
						if (ReadFile(hOutRead, buffer, 4095, &bytesRead, NULL) == NULL) {
							break;
						}
						programOutput += buffer;
					}
					CloseHandle(hOutRead);
				}
				string backupProgramOutput = programOutput; //备份程序输出

				//读取标准输出
				memset(buffer, 0, sizeof buffer);
				string stdOutput = "";

				FILE *fp = fopen(fileout.c_str(), "r");
				char s2;
				while ((s2 = fgetc(fp)) && (s2 != EOF)) {
					stdOutput += s2;
				}
				fclose(fp);
				string backupStdOutput = stdOutput; //备份标准输出
			#pragma endregion
			#pragma region compare
				//比对开始
				int status = NULL;

				for (int i = 0; i < judgeruleslen; i++) {
					//忽略输出末尾的换行
					if (judgerules[i] == 1) {
						string::iterator it;
						for (it = programOutput.end() - 1; it >= programOutput.begin();) {
							if (*it == '\n' || *it == '\r') {
								it = programOutput.erase(it) - 1;
							}
							else {
								break;
							}
						}
						for (it = stdOutput.end() - 1; it >= stdOutput.begin();) {
							if (*it == '\n' || *it == '\r') {
								it = stdOutput.erase(it) - 1;
							}
							else {
								break;
							}
						}
					}
					//忽略每行前端的空格
					else if (judgerules[i] == 2) {
						string::iterator it;
						for (it = programOutput.begin(); it != programOutput.end();) {
							if (*it == '\n') {
								string::iterator it2;
								for (it2 = it + 1; *it2 == ' ';) {
									it2 = programOutput.erase(it2);
								}
								it = it2;
							}
							else if (*it == '\r') {
								it = programOutput.erase(it);
							}
							else {
								it++;
							}
						}
						for (it = programOutput.begin(); it != programOutput.end();) {
							if (*it == ' ') {
								it = programOutput.erase(it);
							}
							else {
								break;
							}
						}
						for (it = stdOutput.begin(); it != stdOutput.end();) {
							if (*it == '\n') {
								string::iterator it2;
								for (it2 = it + 1; *it2 == ' ';) {
									it2 = stdOutput.erase(it2);
								}
								it = it2;
							}
							else if (*it == '\r') {
								it = stdOutput.erase(it);
							}
							else {
								it++;
							}
						}
						for (it = stdOutput.begin(); it != stdOutput.end();) {
							if (*it == ' ') {
								it = stdOutput.erase(it);
							}
							else {
								break;
							}
						}
					}
					//忽略每行末尾的空格
					else if (judgerules[i] == 3) {
						string::iterator it;
						for (it = programOutput.begin(); it != programOutput.end();) {
							if (*it == '\n') {
								string::iterator it2;
								for (it2 = it - 1; *it2 == ' '; it2--) {
									it2 = programOutput.erase(it2);
								}
								it = it2 + 2;
							}
							else if (*it == '\r') {
								it = programOutput.erase(it);
							}
							else {
								it++;
							}
						}
						for (it = programOutput.end() - 1; it >= programOutput.begin();) {
							if (*it == ' ') {
								it = programOutput.erase(it) - 1;
							}
							else {
								break;
							}
						}
						for (it = stdOutput.begin(); it != stdOutput.end();) {
							if (*it == '\n') {
								string::iterator it2;
								for (it2 = it - 1; *it2 == ' '; it2--) {
									it2 = stdOutput.erase(it2);
								}
								it = it2 + 2;
							}
							else if (*it == '\r') {
								it = stdOutput.erase(it);
							}
							else {
								it++;
							}
						}
						for (it = stdOutput.end() - 1; it >= stdOutput.begin();) {
							if (*it == ' ') {
								it = stdOutput.erase(it) - 1;
							}
							else {
								break;
							}
						}
					}
					//忽略重复换行
					else if (judgerules[i] == 4) {
						string::iterator it;
						for (it = programOutput.begin(); it != programOutput.end();) {
							if (*it == '\n') {
								string::iterator it2;
								for (it2 = it + 1; *it2 == '\n';) {
									it2 = programOutput.erase(it2);
								}
								it = it2;
							}
							else if (*it == '\r') {
								it = programOutput.erase(it);
							}
							else {
								it++;
							}
						}
						for (it = stdOutput.begin(); it != stdOutput.end();) {
							if (*it == '\n') {
								string::iterator it2;
								for (it2 = it + 1; *it2 == '\n';) {
									it2 = stdOutput.erase(it2);
								}
								it = it2;
							}
							else if (*it == '\r') {
								it = stdOutput.erase(it);
							}
							else {
								it++;
							}
						}
					}
					//忽略全部空格
					else if (judgerules[i] == 5) {
						string::iterator it;
						for (it = programOutput.begin(); it != programOutput.end();) {
							if (*it == '\n' || *it == '\r') {
								it = programOutput.erase(it);
							}
							else {
								it++;
							}
						}
						for (it = stdOutput.begin(); it != stdOutput.end();) {
							if (*it == '\n' || *it == '\r') {
								it = stdOutput.erase(it);
							}
							else {
								it++;
							}
						}
					}
					//忽略全部换行
					else if (judgerules[i] == 6) {
						string::iterator it;
						for (it = programOutput.begin(); it != programOutput.end();) {
							if (*it == ' ' || *it == '\r') {
								it = programOutput.erase(it);
							}
							else {
								it++;
							}
						}
						for (it = stdOutput.begin(); it != stdOutput.end();) {
							if (*it == ' ' || *it == '\r') {
								it = stdOutput.erase(it);
							}
							else {
								it++;
							}
						}
					}
				}
				
				if (programOutput == stdOutput) {
						status = 1;  // 1 为 AC
				}
				else {
					string::iterator it;
					for (it = programOutput.begin(); it != programOutput.end();) {
						if (*it == '\n' || *it == '\r' || *it == ' ') {
							it = programOutput.erase(it);
						}
						else {
							it++;
						}
					}
					for (it = stdOutput.begin(); it != stdOutput.end();) {
						if (*it == '\n' || *it == '\r' || *it == ' ') {
							it = stdOutput.erase(it);
						}
						else {
							it++;
						}
					}
					if (programOutput == stdOutput) {
						status = 2; //2 为 PE
					}
					else {
						status = 3; //3 为 WA
					}
				}
			#pragma endregion
			#pragma region report
				if (status == 1) {
					printfcolor(COLOR_GREEN, "数据 %d 答案正确 AC", cnt);
					fprintf(outputrs, "数据 %d | 文件 %s 答案正确 AC 耗时 %d ms 内存 %d kB\n", cnt, filein.c_str(), ms, kb);
					AC++;
				}
				else if (status == 2) {
					printfcolor(COLOR_YELLOW, "数据 %d 格式错误 PE", cnt);
					fprintf(outputrs, "数据 %d | 文件 %s 格式错误 PE 耗时 %d ms 内存 %d kB\n", cnt, filein.c_str(), ms, kb);
					fprintf(outputrs, "-----\n");
					fprintf(outputrs, "标准输出 stdOutput: \n");
					fprintf(outputrs, "%s\n", backupStdOutput.c_str());
					fprintf(outputrs, "程序输出 programOutput: \n");
					fprintf(outputrs, "%s\n", backupProgramOutput.c_str());
					fprintf(outputrs, "-----\n");
					PE++;
				}
				else if (status == 3) {
					printfcolor(COLOR_RED, "数据 %d 答案错误 WA", cnt);
					fprintf(outputrs, "数据 %d | 文件 %s 答案错误 WA 耗时 %d ms 内存 %d kB\n", cnt, filein.c_str(), ms, kb);
					fprintf(outputrs, "-----\n");
					fprintf(outputrs, "标准输出 stdOutput: \n");
					fprintf(outputrs, "%s\n", backupStdOutput.c_str());
					fprintf(outputrs, "程序输出 programOutput: \n");
					fprintf(outputrs, "%s\n", backupProgramOutput.c_str());
					fprintf(outputrs, "-----\n");
					WA++;
				}

				printf(" 耗时 %d ms 内存 %d kB\n", ms, kb);
			#pragma endregion
			}
	#pragma region reportother
			else if (rs == 2) {
				printfcolor(COLOR_BLUE, "数据 %d 时间超限 TLE\n", cnt);
				fprintf(outputrs, "数据 %d | 文件 %s 时间超限 TLE\n", cnt, filein.c_str());
				TLE++;
			}
			else if (rs == 3) {
				printfcolor(COLOR_CYAN, "数据 %d 运行错误 RE\n", cnt);
				fprintf(outputrs, "数据 %d | 文件 %s 运行错误 RE\n", cnt, filein.c_str());
				RE++;
			}
			else if (rs == 4) {
				printfcolor(COLOR_ORGINAL, "数据 %d 内存超限 MLE 内存 %d kB\n", cnt);
				fprintf(outputrs, "数据 %d | 文件 %s 内存超限 MLE 内存 %d kB\n", cnt, filein.c_str(), kb);
				MLE++;
			}
			fprintf(outputrs, "\n");
	#pragma endregion
	#pragma region deletetmpfile
			if (isfreopen) {
				if (DeleteFile(targetinput.c_str()) == 0 || DeleteFile(targetoutput.c_str()) == 0) {
					printferror("删除临时文件失败！\n");
					printf("如果要继续，");
					pressAnyButtonTo("继续");
				}
			}
	#pragma endregion
			Sleep(20);
			cnt++;
		}
	#pragma endregion
	#pragma region afterall
		/*
		完成后
		*/
		printf("\n评测结束\n");
		printf("\n");
		if (AC != 0) {
			printfcolor(COLOR_GREEN, "答案正确 AC: %d\n", AC);
			fprintf(outputrs, "答案正确 AC: %d\n", AC);
		}
		if (WA != 0) {
			printfcolor(COLOR_RED, "答案错误 WA: %d\n", WA);
			fprintf(outputrs, "答案错误 WA: %d\n", WA);
		}
		if (PE != 0) {
			printfcolor(COLOR_YELLOW, "格式错误 PE: %d\n", PE);
			fprintf(outputrs, "格式错误 PE: %d\n", PE);
		}
		if (RE != 0) {
			printfcolor(COLOR_CYAN, "运行错误 RE: %d\n", RE);
			fprintf(outputrs, "运行错误 RE: %d\n", RE);
		}
		if (TLE != 0) {
			printfcolor(COLOR_BLUE, "时间超限 TLE: %d\n", TLE);
			fprintf(outputrs, "时间超限 TLE: %d\n", TLE);
		}
		if (MLE != 0) {
			printfcolor(COLOR_CYAN, "内存超限 MLE: %d\n", MLE);
			fprintf(outputrs, "内存超限 MLE: %d\n", MLE);
		}
		fclose(outputrs);
		printf("\n");
		string tmppath = ".\\tmp\\";
		tmppath += freopenstdin;
		DeleteFile(tmppath.c_str());
		tmppath = ".\\tmp\\";
		tmppath += freopenstdout;
		DeleteFile(tmppath.c_str());

		if (DeleteFile(".\\tmp\\program.exe") == 0) {
			printferror("删除临时编译程序失败！\n");
		}

		if (_rmdir("tmp") == -1) {
			printferror("删除临时目录 tmp 失败！请手动删除！\n");
		}
		fcloseall();
	#pragma endregion
	#pragma region exit
		pressAnyButtonTo("退出");
		exit(0);
	#pragma endregion
	}
#pragma endregion
}
