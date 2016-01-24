/*
	@package MuJudge-Console
	@version 1.3
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

#define VERSION "1.3"

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

#pragma region vars
char pathofgcc[MAXN] = { 0 }, formatofin[MAXN] = { 0 }, formatofout[MAXN] = { 0 }, tmpmsecond[MAXN] = { 0 }, tmpjudgerules[MAXN] = { 0 }, tmpmemorylimit[MAXN] = { 0 };
char pathofgccinput[MAXN] = { 0 }, tmpjudgemode[MAXN] = { 0 }, pathofsource[MAXN];
bool isconfig = false, tmpisconfig = false, isfreopen = false, nocomplier = false, isexe = false, isexit = false, isexe1 = false, isexe2 = false;
char freopenstdin[MAXN], freopenstdout[MAXN], inputmainfilename[MAXN], inputmainfilesuffix[MAXN], outputmainfilename[MAXN], outputmainfilesuffix[MAXN];
int startnum = 0, endnum = 0, startnumlen = 0, endnumlen = 0, tmpinputlen = 0, msecond = INFINITE;
long long memorylimit = NULL;
int judgerules[MAXN] = { 0 }, judgeruleslen = 0, judgemode = 1;
char pathofsourcefirst[MAXN] = { 0 }, pathofsourcesecond[MAXN] = { 0 };
bool sourcefirstisfreopen, sourcesecondisfreopen;
char sourcefirstfreopenstdin[MAXN], sourcefirstfreopenstdout[MAXN], sourcesecondfreopenstdin[MAXN], sourcesecondfreopenstdout[MAXN];
bool isfreopenstdinsame = false, isfreopenstdoutsame = false;
#pragma endregion

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
void pressAnyButtonTo(char* info) {
	printf("请按任意键");
	printf(info);
	rewind(stdin);
	_getch();
}
void getCString(char* s) {
	memset(s, 0, sizeof s);
	rewind(stdin);
	char l;
	int len = 0;
	while ((l = getchar()) && (l != EOF) && (l != '\n') && (l != '\r')) {
		s[len++] = l;
	}
	s[len] = 0;
	len--;
	while (s[len] == '\n' || s[len] == ' ') {
		s[len] = 0;
		len--;
	}
	rewind(stdin);
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
void deleteTmpInOutFile() {
	string deletefile;
	if (judgemode == 1) {
		if (isfreopen) {
			deletefile = ".\\tmp\\";
			deletefile += freopenstdin;
			if (DeleteFile(deletefile.c_str()) == 0) {
				printferror("删除临时输入文件失败！\n");
			}
			deletefile = ".\\tmp\\";
			deletefile += freopenstdout;
			if (DeleteFile(deletefile.c_str()) == 0) {
				int code = GetLastError();
				if (code == 2) {
					printferror("无输出文件！\n");
				}
				else {
					printferror("删除临时输出文件失败！\n");
				}


			}
		}
	}
	else if (judgemode == 2) {
		if (sourcefirstisfreopen) {
			deletefile = ".\\tmp\\";
			deletefile += sourcefirstfreopenstdin;
			if (DeleteFile(deletefile.c_str()) == 0) {
				printferror("删除临时程序一输入文件失败！\n");
			}
			deletefile = ".\\tmp\\";
			deletefile += sourcefirstfreopenstdout;
			if (DeleteFile(deletefile.c_str()) == 0) {
				int code = GetLastError();
				if (code == 2) {
					printferror("程序一无输出文件！\n");
				}
				else {
					printferror("删除临时程序一输出文件失败！\n");
				}
			}
		}
		if (sourcesecondisfreopen) {
			if (!isfreopenstdinsame) {
				deletefile = ".\\tmp\\";
				deletefile += sourcesecondfreopenstdin;
				if (DeleteFile(deletefile.c_str()) == 0) {
					printferror("删除临时程序二输入文件失败！\n");
				}
			}
			if (!isfreopenstdoutsame) {
				deletefile = ".\\tmp\\";
				deletefile += sourcesecondfreopenstdout;
				if (DeleteFile(deletefile.c_str()) == 0) {
					int code = GetLastError();
					if (code == 2) {
						printferror("程序二无输出文件！\n");
					}
					else {
						printferror("删除临时程序二输出文件失败！\n");
					}
				}
			}
		}
	}
}
void deleteTmpProgram() {
	if (judgemode == 1) {
		if (DeleteFile(".\\tmp\\program.exe") == 0) {
			printferror("删除临时编译程序失败！\n");
		}
	}
	else if (judgemode == 2) {
		if (DeleteFile(".\\tmp\\program1.exe") == 0) {
			printferror("删除临时编译程序一失败！\n");
		}
		if (DeleteFile(".\\tmp\\program2.exe") == 0) {
			printferror("删除临时编译程序二失败！\n");
		}
	}
}
void deleteTmpDir() {
	if (RemoveDirectory(".\\tmp") == 0) {
		printferror("删除临时文件夹失败，请自行删除\n");
	}
}
void noticeandexit(int exitcode) {
	fcloseall();

	deleteTmpProgram();
	deleteTmpInOutFile();
	deleteTmpDir();
	
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
bool InJudgeCtrlCHandle(DWORD handlertype) {
	switch (handlertype){
		case CTRL_C_EVENT:
			isexit = true;
			return true;
		default:
			return false;
	}
}
bool OutJudgeCtrlCHandle(DWORD handlertype) {
	switch (handlertype) {
		case CTRL_C_EVENT:
			printf("你按下了Ctrl + C，但是在不在评测时然并卵，如果要退出直接右上角\n");
			return true;
		default:
			return false;
	}
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
string readOutput(HANDLE hOutRead, bool isfreopen, char freopenstdout[]) {
	char buffer[4096] = { 0 };
	DWORD bytesRead;
	string programOutput = "", targetoutput = ".\\tmp\\";
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
	return programOutput; //备份程序输出
}
#pragma endregion

int main(int argc, char* argv[], char* envp[]) {
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)OutJudgeCtrlCHandle, true);
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
		printf("3. 时间测试可能不准确\n");
		printf("4. 已加入 exe 文件评测\n");
		printf("5. 已加入对拍评测，请预先准备好数据\n");
		setFontColor(COLOR_WHITE);
	#pragma endregion
	#pragma region config
		/*
		配置部分
		*/
		

		#pragma region readautoconfig
		//检测读取配置
		FILE *fp = fopen("MuJudge-Console_Config.txt", "r");
		if (fp != NULL) {
			printfcolor(COLOR_CYAN, "\n找到基础配置文件，是否读取配置，回车即为是\n");
			while (true) {
				setFontColor(COLOR_WHITE);
				printf("readconfig[y/n]: ");
				char c = getchar();
				rewind(stdin);

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
					else if (tmpstr.substr(0, 11) == "judgemode: ") {
						memset(tmpjudgemode, 0, sizeof tmpjudgemode);
						strcpy(tmpjudgemode, tmpstr.substr(11, tmpstr.length() - 11).c_str());
					}
					else if (tmpstr.substr(0, 10) == "judgemode:") {
						memset(tmpjudgemode, 0, sizeof tmpjudgemode);
						strcpy(tmpjudgemode, tmpstr.substr(10, tmpstr.length() - 10).c_str());
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
						memset(tmpmemorylimit, 0, sizeof tmpmemorylimit);
						strcpy(tmpmemorylimit, tmpstr.substr(13, tmpstr.length() - 13).c_str());
					}
					else if (tmpstr.substr(0, 12) == "memorylimit:") {
						memset(tmpmemorylimit, 0, sizeof tmpmemorylimit);
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
		printfcolor(COLOR_CYAN, "\n请输入MinGW下的bin路径，像D:\\example\\bin这样。如果设置了MinGW环境变量或者其它类型gcc编译器，MinGW环境变量一定要为*\\MinGW\\bin，请直接回车，如果没有编译器，使用exe直接评测，请输入 none\n");
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
			if (strcmp(pathofgcc, "none") == 0) {
				nocomplier = true;
				strcpy(pathofgccinput, "none");
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

						rewind(stdin);
						char tmpc = getchar();
						rewind(stdin);
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
		#pragma region config_judgemode
		//Judge模式设置，对拍和标准输出
		printfcolor(COLOR_CYAN, "\n请输入序号来选择评测模式，直接回车即为标准输出文件模式\n");
		printf("1. 标准输出文件模式 STDInputOutputFileMode\n");
		printf("2. 对拍模式 ComparisonMode\n");
		tmpisconfig = isconfig;
		judgemode = 1;
		while (true) {
			setFontColor(COLOR_WHITE);
			printf("judgemode: ");
			if (tmpisconfig) {
				printf(tmpjudgemode);
				printf("\n");
				tmpisconfig = false;
			}
			else {
				getCString(tmpjudgemode);
			}

			setFontColor(COLOR_RED);
			char inpu = tmpjudgemode[0];
			if (inpu == 0 || inpu == '1') {
				judgemode = 1;
			}
			else if (inpu == '2') {
				judgemode = 2;
			}
			else {
				printf("你的输入有误，请检查后重新输入\n");
				continue;
			}
			break;
		}
		#pragma endregion
		#pragma region otherconfig
		if (judgemode == 1) {
			#pragma region config_inputfile
			//输入文件检查
			printfcolor(COLOR_CYAN, "\n请输入输入文件的文件名或路径，有后缀名一定要包含后缀名，用[0]来表示数字递增从0开始，用[00]来表示数字递增从00开始，用[1-100]来表示数字只在1和100之间，首个输入文件必须存在\n");
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
			printfcolor(COLOR_CYAN, "\n请输入最大内存限制，以千字节(kB)为单位，如果要使用兆字节(MB)作为单位，请在数字后面加上M，回车即为默认 128M\n");
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
				bool istrue = true;
				for (int i = 0; i < tmpmemorylimitlen; i++) {
					if (isdigit(tmpmemorylimit[i])) {
						memorylimit = memorylimit * 10 + tmpmemorylimit[i] - '0';
					}
					else if (tolower(tmpmemorylimit[i]) == 'k') {
						break;
					}
					else if (tolower(tmpmemorylimit[i]) == 'm') {
						memorylimit *= 1024;
						break;
					}
					else {
						printf("输入错误，请重新输入！\n");
						istrue = false;
						break;
					}
				}
				if (!istrue) {
					continue;
				}
				break;
			}
			#pragma endregion
			#pragma region config_judgerules
			//评测模式设置
			printfcolor(COLOR_CYAN, "\n请输入数字序号来设置评测规则，多个规则请连写，如123，直接回车则 100%% 相同则 AC，所有规则可重复，且有顺序处理，且同时处理标准输出和程序输出\n");
			printf("1. 忽略输出末尾的换行;\n");
			printf("2. 忽略每行前端的空格;\n");
			printf("3. 忽略每行末尾的空格;\n");
			printf("4. 忽略重复换行;\n");
			printf("5. 忽略所有空格;\n");
			printf("6. 忽略所有换行;\n");
		
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
			fprintf(fp, "judgemode: %d\n", judgemode);
			fprintf(fp, "fileofin: %s\n", formatofin);
			fprintf(fp, "fileofout: %s\n", formatofout);
			char msecondstrtmp[MAXN];
			itoa(msecond, msecondstrtmp, 10);
			fprintf(fp, "timeout: %s\n", msecond == INFINITE ? "infinite" : msecondstrtmp);
			fprintf(fp, "memorylimit: %I64d\n", memorylimit);
			fprintf(fp, "judgerules: %s\n", tmpjudgerules);
			fclose(fp);
			printf("\n基础配置保存成功！\n");
			#pragma endregion
			#pragma region config_source
			//源程序存在性检测
			printfcolor(COLOR_CYAN, "\n请输入源文件名或路径，源文件名应该是这样的：example.c或者example.cpp。如果没有设置 gcc 路径为 none, 然后使用 exe 文件进行评测，像这样 example.exe\n");
			while (true) {
				setFontColor(COLOR_WHITE);
				printf("source: ");
				getCString(pathofsource);

				setFontColor(COLOR_RED);
				int pathofsourcelen = strlen(pathofsource);

				bool isright = false;
				char suffix[10];
				//检查后缀是否为.c或者.cpp或者.exe
				if (pathofsourcelen > 4) {
					memset(suffix, 0, sizeof suffix);
					substr(suffix, pathofsource, pathofsourcelen - 4, 4);
					if (strcmp(suffix, ".cpp") != 0 && strcmp(suffix, ".exe") != 0) {
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
					if (strcmp(pathofsource, ".c") == 0 || strcmp(pathofsource, ".cpp") == 0 || strcmp(pathofsource, ".exe") == 0) {
						printf("你输入的源文件名有误，请检查后重新输入\n");
					}
					else {
						printf("你输入的源文件名或路径后缀不为.c或者.cpp或者.exe，请检查后重新输入\n");
					}
					continue;
				}
				if ((strcmp(suffix, ".cpp") == 0 || strcmp(suffix, ".c") == 0) && nocomplier) {
					printf("你没有设置编译器，不能使用.cpp或者.c源文件，请检查后重新输入exe文件\n");
					continue;
				}
				if (strcmp(suffix, ".exe") == 0) {
					isexe = true;
					break;
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
			while (true) {
				setFontColor(COLOR_WHITE);
				printf("freopen or others[y/n]: ");

				rewind(stdin);
				char c = getchar();
				rewind(stdin);

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
			printf("judgemode: %s\n", judgemode == 1 ? "STDInputOutputFileMode" : "ComparisonMode");
			printf("fileofin: %s\n", formatofin);
			printf("fileofout: %s\n", formatofout);
			memset(msecondstrtmp, 0, sizeof msecondstrtmp);
			itoa(msecond, msecondstrtmp, 10);
			printf("timeout: %s\n", msecond == INFINITE ? "infinite" : msecondstrtmp);
			printf("memorylimit: %I64d kB\n", memorylimit);
			printf("judgerules: %s\n", tmpjudgerules);
			printf("-----\n");
			printf("source: %s\n", pathofsource);
			printf("freopen or others[y/n]: %s\n", isfreopen ? "y" : "n");
			if (isfreopen) {
				printf("freopen or others stdin: %s\n", freopenstdin);
				printf("freopen or others stdout: %s\n", freopenstdout);
			}
			setFontColor(COLOR_WHITE);
		#pragma endregion
		}
		else if (judgemode == 2) {
			#pragma region config_inputfile
			//输入文件检查
			printfcolor(COLOR_CYAN, "\n请输入输入文件的文件名或路径，有后缀名一定要包含后缀名，用[0]来表示数字递增从0开始，用[00]来表示数字递增从00开始，用[1-100]来表示数字只在1和100之间，首个输入文件必须存在\n");
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
				if (error == 2) {
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
			#pragma region config_timeout
			//超时设置
			printfcolor(COLOR_CYAN, "\n请输入程序超时时间，以毫秒(ms)为单位，必须为整数，直接回车或者输入 0 则为不限时间\n");
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
			printfcolor(COLOR_CYAN, "\n请输入最大内存限制，以千字节(kB)为单位，如果要使用兆字节(MB)作为单位，请在数字后面加上M，回车即为默认 128M\n");
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
				bool istrue = true;
				for (int i = 0; i < tmpmemorylimitlen; i++) {
					if (isdigit(tmpmemorylimit[i])) {
						memorylimit = memorylimit * 10 + tmpmemorylimit[i] - '0';
					}
					else if (tolower(tmpmemorylimit[i]) == 'k') {
						break;
					}
					else if (tolower(tmpmemorylimit[i]) == 'm') {
						memorylimit *= 1024;
						break;
					}
					else {
						printf("输入错误，请重新输入！\n");
						istrue = false;
						break;
					}
				}
				if (!istrue) {
					continue;
				}
				break;
			}
			#pragma endregion
			#pragma region config_judgerules
			//评测模式设置
			printfcolor(COLOR_CYAN, "\n请输入数字序号来设置评测规则，多个规则请连写，如123，直接回车则 100%% 相同则 AC，所有规则可重复，且有顺序处理，且同时处理标准输出和程序输出\n");
			printf("1. 忽略输出末尾的换行;\n");
			printf("2. 忽略每行前端的空格;\n");
			printf("3. 忽略每行末尾的空格;\n");
			printf("4. 忽略重复换行;\n");
			printf("5. 忽略所有空格;\n");
			printf("6. 忽略所有换行;\n");
		
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
			fprintf(fp, "judgemode: %d\n", judgemode);
			fprintf(fp, "fileofin: %s\n", formatofin);
			char msecondstrtmp[MAXN];
			itoa(msecond, msecondstrtmp, 10);
			fprintf(fp, "timeout: %s\n", msecond == INFINITE ? "infinite" : msecondstrtmp);
			fprintf(fp, "memorylimit: %I64d\n", memorylimit);
			fprintf(fp, "judgerules: %s\n", tmpjudgerules);
			fclose(fp);
			printf("\n基础配置保存成功！\n");
			#pragma endregion
			#pragma region config_source
			//源程序存在性检测
			printfcolor(COLOR_CYAN, "\n请输入第一个源文件名或路径，源文件名应该是这样的：example.c或者example.cpp。如果没有设置 gcc 路径为 none, 然后使用 exe 文件进行评测，像这样 example.exe。一般第一个程序为标准程序\n");
			while (true) {
				setFontColor(COLOR_WHITE);
				printf("source-first: ");
				getCString(pathofsourcefirst);

				setFontColor(COLOR_RED);
				int pathofsourcefirstlen = strlen(pathofsourcefirst);

				bool isright = false;
				char suffix[10];
				//检查后缀是否为.c或者.cpp或者.exe
				if (pathofsourcefirstlen > 4) {
					memset(suffix, 0, sizeof suffix);
					substr(suffix, pathofsourcefirst, pathofsourcefirstlen - 4, 4);
					if (strcmp(suffix, ".cpp") != 0 && strcmp(suffix, ".exe") != 0) {
						memset(suffix, 0, sizeof suffix);
						substr(suffix, pathofsourcefirst, pathofsourcefirstlen - 2, 2);
						if (strcmp(suffix, ".c") == 0) isright = true;
					}
					else isright = true;
				}
				else if (pathofsourcefirstlen > 2) {
					char suffix[10];
					memset(suffix, 0, sizeof suffix);
					substr(suffix, pathofsourcefirst, pathofsourcefirstlen - 2, 2);
					if (strcmp(suffix, ".c") == 0) isright = true;
				}
				if (!isright) {
					if (strcmp(pathofsourcefirst, ".c") == 0 || strcmp(pathofsourcefirst, ".cpp") == 0 || strcmp(pathofsourcefirst, ".exe") == 0) {
						printf("你输入的源文件名有误，请检查后重新输入\n");
					}
					else {
						printf("你输入的源文件名或路径后缀不为.c或者.cpp或者.exe，请检查后重新输入\n");
					}
					continue;
				}
				if ((strcmp(suffix, ".cpp") == 0 || strcmp(suffix, ".c") == 0) && nocomplier) {
					printf("你没有设置编译器，不能使用.cpp或者.c源文件，请检查后重新输入exe文件\n");
					continue;
				}
				if (strcmp(suffix, ".exe") == 0) {
					isexe1 = true;
					break;
				}

				FILE *fp = fopen(pathofsourcefirst, "r");
				if (fp == NULL) {
					printf("源文件不存在，请检查后重新输入\n");
					continue;
				}
				else fclose(fp);
				break;
			}
			printfcolor(COLOR_CYAN, "\n请输入第二个源文件名或路径，源文件名应该是这样的：example.c或者example.cpp。如果没有设置 gcc 路径为 none, 然后使用 exe 文件进行评测，像这样 example.exe。一般第二个程序为被评测程序\n");
			while (true) {
				setFontColor(COLOR_WHITE);
				printf("source-second: ");
				getCString(pathofsourcesecond);

				setFontColor(COLOR_RED);
				int pathofsourcesecondlen = strlen(pathofsourcesecond);

				bool isright = false;
				char suffix[10];
				//检查后缀是否为.c或者.cpp或者.exe
				if (pathofsourcesecondlen > 4) {
					memset(suffix, 0, sizeof suffix);
					substr(suffix, pathofsourcesecond, pathofsourcesecondlen - 4, 4);
					if (strcmp(suffix, ".cpp") != 0 && strcmp(suffix, ".exe") != 0) {
						memset(suffix, 0, sizeof suffix);
						substr(suffix, pathofsourcesecond, pathofsourcesecondlen - 2, 2);
						if (strcmp(suffix, ".c") == 0) isright = true;
					}
					else isright = true;
				}
				else if (pathofsourcesecondlen > 2) {
					char suffix[10];
					memset(suffix, 0, sizeof suffix);
					substr(suffix, pathofsourcesecond, pathofsourcesecondlen - 2, 2);
					if (strcmp(suffix, ".c") == 0) isright = true;
				}
				if (!isright) {
					if (strcmp(pathofsourcesecond, ".c") == 0 || strcmp(pathofsourcesecond, ".cpp") == 0 || strcmp(pathofsourcesecond, ".exe") == 0) {
						printf("你输入的源文件名有误，请检查后重新输入\n");
					}
					else {
						printf("你输入的源文件名或路径后缀不为.c或者.cpp或者.exe，请检查后重新输入\n");
					}
					continue;
				}
				if ((strcmp(suffix, ".cpp") == 0 || strcmp(suffix, ".c") == 0) && nocomplier) {
					printf("你没有设置编译器，不能使用.cpp或者.c源文件，请检查后重新输入exe文件\n");
					continue;
				}
				if (strcmp(suffix, ".exe") == 0) {
					isexe2 = true;
					break;
				}

				FILE *fp = fopen(pathofsourcesecond, "r");
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
			printfcolor(COLOR_CYAN, "\n请选择你是否已经在第一个程序中加入重定向标准输入输出流的语句，如freopen，回车即为否\n");
			while (true) {
				setFontColor(COLOR_WHITE);
				printf("source-first freopen or others[y/n]: ");

				rewind(stdin);
				char c = getchar();
				rewind(stdin);

				setFontColor(COLOR_RED);
				if (c == 'Y' || c == 'y') {
					sourcefirstisfreopen = true;
				}
				else if (c == 'N' || c == 'n' || c == '\n') {
					sourcefirstisfreopen = false;
				}
				else {
					printf("你输入的有误，请重新输入\n");
					continue;
				}
				break;
			}
			if (sourcefirstisfreopen) {
				printfcolor(COLOR_CYAN, "\n请输入重定向标准输入的替代文件名（本句没有安全性检查）\n");
				while (true) {
					setFontColor(COLOR_WHITE);
					printf("source-first freopen or others stdin: ");
					getCString(sourcefirstfreopenstdin);

					setFontColor(COLOR_RED);
					if (sourcefirstfreopenstdin[0] == 0){
						printf("你没有输入，请重新输入\n");
						continue;
					}
					break;
				}

				printfcolor(COLOR_CYAN, "\n请输入重定向标准输出的替代文件名（本句没有安全性检查）\n");
				while (true) {
					setFontColor(COLOR_WHITE);
					printf("source-first freopen or others stdout: ");
					getCString(sourcefirstfreopenstdout);

					setFontColor(COLOR_RED);
					if (sourcefirstfreopenstdout[0] == 0){
						printf("你没有输入，请重新输入\n");
						continue;
					}
					break;
				}
			}
			printfcolor(COLOR_CYAN, "\n请选择你是否已经在第二个程序中加入重定向标准输入输出流的语句，如freopen，回车即为否\n");
			while (true) {
				setFontColor(COLOR_WHITE);
				printf("source-second freopen or others[y/n]: ");

				rewind(stdin);
				char c = getchar();
				rewind(stdin);

				setFontColor(COLOR_RED);
				if (c == 'Y' || c == 'y') {
					sourcesecondisfreopen = true;
				}
				else if (c == 'N' || c == 'n' || c == '\n') {
					sourcesecondisfreopen = false;
				}
				else {
					printf("你输入的有误，请重新输入\n");
					continue;
				}
				break;
			}
			if (sourcesecondisfreopen) {
				printfcolor(COLOR_CYAN, "\n请输入重定向标准输入的替代文件名，直接回车即为与第一个文件相同（本句没有安全性检查）\n");
				while (true) {
					setFontColor(COLOR_WHITE);
					printf("source-second freopen or others stdin: ");
					getCString(sourcesecondfreopenstdin);

					if (sourcesecondfreopenstdin[0] == 0){
						memset(sourcesecondfreopenstdin, 0, sizeof sourcefirstfreopenstdin);
						strcpy(sourcesecondfreopenstdin, sourcefirstfreopenstdin);
					}
					break;
				}

				printfcolor(COLOR_CYAN, "\n请输入重定向标准输出的替代文件名，直接回车即为与第一个文件相同（本句没有安全性检查）\n");
				while (true) {
					setFontColor(COLOR_WHITE);
					printf("source-second freopen or others stdout: ");
					getCString(sourcesecondfreopenstdout);

					if (sourcesecondfreopenstdout[0] == 0){
						memset(sourcesecondfreopenstdout, 0, sizeof sourcefirstfreopenstdout);
						strcpy(sourcesecondfreopenstdout, sourcefirstfreopenstdout);
					}
					break;
				}
			}
			isfreopenstdinsame = isfreopenstdoutsame = false;
			if (strcmp(sourcefirstfreopenstdin, sourcesecondfreopenstdin) == 0)
				isfreopenstdinsame = true;
			if (strcmp(sourcefirstfreopenstdout, sourcesecondfreopenstdout) == 0)
				isfreopenstdoutsame = true;
			#pragma endregion
			#pragma region showconfig
			clrscr();
			printTitle();
			setFontColor(COLOR_PINK);
			printf("你输入的信息：\n");
			printf("gcc: %s\n", pathofgccinput);
			printf("judgemode: %s\n", judgemode == 1 ? "STDOutputFileMode" : "ComparisonMode");
			printf("fileofin: %s\n", formatofin);
			memset(msecondstrtmp, 0, sizeof msecondstrtmp);
			itoa(msecond, msecondstrtmp, 10);
			printf("timeout: %s\n", msecond == INFINITE ? "infinite" : msecondstrtmp);
			printf("memorylimit: %I64d kB\n", memorylimit);
			printf("judgerules: %s\n", tmpjudgerules);
			printf("-----\n");
			printf("source-first: %s\n", pathofsourcefirst);
			printf("source-second: %s\n", pathofsourcesecond);
			printf("source-first freopen or others[y/n]: %s\n", isfreopen ? "y" : "n");
			if (sourcefirstisfreopen) {
				printf("source-first freopen or others stdin: %s\n", sourcefirstfreopenstdin);
				printf("source-first freopen or others stdout: %s\n", sourcefirstfreopenstdout);
			}
			printf("source-second freopen or others[y/n]: %s\n", isfreopen ? "y" : "n");
			if (sourcesecondisfreopen) {
				printf("source-second freopen or others stdin: %s\n", sourcesecondfreopenstdin);
				printf("source-second freopen or others stdout: %s\n", sourcesecondfreopenstdout);
			}
			setFontColor(COLOR_WHITE);
		#pragma endregion
		}
		#pragma endregion
	#pragma endregion
	#pragma region createtmpdir
		if (!CheckFolderExist("tmp")) {
			if (_mkdir("tmp") == -1) {
				printf("创建tmp目录失败，请手动创建一个！\n");
				noticeandexit(1);
			}
		}
	#pragma endregion
	#pragma region complier
		/*
		编译部分
		*/
		if (judgemode == 2) {
			if (!isexe1) {
				printf("\n开始编译程序一\n");
				#pragma region docomplie_source_one
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
				sprintf(cmdline, "%s %s -o .\\tmp\\program1.exe -fno-asm -O2 -Wall -lm --static", pathofgcc, pathofsourcefirst);

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
						printferror("程序一编译错误！请检查环境变量是否真正设置正确！\n");
					}
					else {
						printferror("程序一编译错误！请检查编译器是否存在。\n");
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
						printf("程序一编译错误！\n");
						setFontColor(COLOR_WHITE);
					}
					else {
						setFontColor(COLOR_RED);
						printf("程序一编译结束，但存在 %d 个错误", cnterr);
						if (cntwarn != 0) {
							setFontColor(COLOR_YELLOW);
							printf(" %d 个警告", cntwarn);
						}
						printf("\n");
						setFontColor(COLOR_WHITE);
					}
					writeToFile("MuJudge-Console_Compilerlog_Program1.txt", strOutput.c_str(), true);
					printf("程序一编译信息已经保存在当前目录MuJudge-Console_Compilerlog_Program1.txt中，请修正错误后再来Judge！\n");
					noticeandexit(1);
				}
				else if (exitcode == 0) {
					if (cntwarn == 0) {
						printf("程序一编译成功！\n");
					}
					else {
						setFontColor(COLOR_YELLOW);
						printf("程序一编译结束，但存在 %d 个警告\n", cntwarn);
						setFontColor(COLOR_WHITE);
						writeToFile("MuJudge-Console_Compilerlog_Program1.txt", strOutput.c_str(), true);
						printf("程序一编译信息已经保存在当前目录MuJudge-Console_Compilerlog_Program1.txt中\n");
					}
				}
				printf("\n");
				#pragma endregion
			}
			else {
				printf("\n将 程序一 exe 文件复制到临时目录\n");
				int returncode = CopyFile(pathofsourcefirst, ".\\tmp\\program1.exe", false);
				if (returncode == 0) {
					printferror("复制 程序一 exe 文件时出错！\n");
					printferror("错误代码: %d\n", GetLastError());
					noticeandexit(1);
				}
				printf("程序一 复制成功！\n\n");
			}
			if (!isexe2) {
				printf("\n开始编译程序二\n");
				#pragma region docomplie_source_two
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
				sprintf(cmdline, "%s %s -o .\\tmp\\program2.exe -fno-asm -O2 -Wall -lm --static", pathofgcc, pathofsourcesecond);

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
						printferror("程序二编译错误！请检查环境变量是否真正设置正确！\n");
					}
					else {
						printferror("程序二编译错误！请检查编译器是否存在。\n");
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
						printf("程序二编译错误！\n");
						setFontColor(COLOR_WHITE);
					}
					else {
						setFontColor(COLOR_RED);
						printf("程序二编译结束，但存在 %d 个错误", cnterr);
						if (cntwarn != 0) {
							setFontColor(COLOR_YELLOW);
							printf(" %d 个警告", cntwarn);
						}
						printf("\n");
						setFontColor(COLOR_WHITE);
					}
					writeToFile("MuJudge-Console_Compilerlog_Program2.txt", strOutput.c_str(), true);
					printf("程序二编译信息已经保存在当前目录MuJudge-Console_Compilerlog_Program2.txt中，请修正错误后再来Judge！\n");
					noticeandexit(1);
				}
				else if (exitcode == 0) {
					if (cntwarn == 0) {
						printf("程序二编译成功！\n");
					}
					else {
						setFontColor(COLOR_YELLOW);
						printf("程序二编译结束，但存在 %d 个警告\n", cntwarn);
						setFontColor(COLOR_WHITE);
						writeToFile("MuJudge-Console_Compilerlog_Program2.txt", strOutput.c_str(), true);
						printf("程序二编译信息已经保存在当前目录MuJudge-Console_Compilerlog_Program2.txt中\n");
					}
				}
				printf("\n");
				#pragma endregion
			}
			else {
				printf("\n将 程序二 exe 文件复制到临时目录\n");
				int returncode = CopyFile(pathofsourcesecond, ".\\tmp\\program2.exe", false);
				if (returncode == 0) {
					printferror("复制 程序二 exe 文件时出错！\n");
					printferror("错误代码: %d\n", GetLastError());
					noticeandexit(1);
				}
				printf("程序二 复制成功！\n\n");
			}
		}
		else if (judgemode == 1) {
			if (!isexe) {
				printf("\n开始编译被测程序\n");
			#pragma region docomplie_source
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
			}
			else {
				printf("\n将 exe 文件复制到临时目录\n");
				int returncode = CopyFile(pathofsource, ".\\tmp\\program.exe", false);
				if (returncode == 0) {
					printferror("复制 exe 文件时出错！\n");
					printferror("错误代码: %d\n", GetLastError());
					noticeandexit(1);
				}
				printf("复制成功！\n\n");
			}
		}
	#pragma endregion
	#pragma region judge
		/*
		Judge部分
		*/
		pressAnyButtonTo("开始评测");
		SetConsoleCtrlHandler((PHANDLER_ROUTINE)OutJudgeCtrlCHandle, false);
		SetConsoleCtrlHandler((PHANDLER_ROUTINE)InJudgeCtrlCHandle, true);
		printf("\n开始评测\n");
		printf("\n");
		int cnt = startnum;
		char format[MAXN] = "%0";
		char tmp[MAXN];
		memset(tmp, 0, sizeof tmp);
		itoa(startnumlen, tmp, 10);
		strcat(format, tmp);
		strcat(format, "d");
		FILE *outputrs;
		outputrs = fopen("MuJudge-Console_JudgeResult.txt", "w+");
		fprintf(outputrs, "MuJudge-Console 评测结果\n\n");
		int AC, WA, PE, TLE, RE, MLE;
		int SA, TDF, PDF; //SA = same TDF = totally different PDF Presentation different
		if (judgemode == 1) {
			AC = WA = PE = TLE = RE = MLE = 0;
			while (true) {
				#pragma region breakjudge
				if (isexit) {
					printfcolor(COLOR_YELLOW, "你按下了Ctrl + C，确定要中断评测吗？回车默认为不中断\n");
					while (true) {
						setFontColor(COLOR_WHITE);
						printf("breakjudge[y/n]: ");
						rewind(stdin);
						char inp = getchar();
						rewind(stdin);

						setFontColor(COLOR_RED);
						if (tolower(inp) == 'y') {
							printf("用户中断了评测！\n");
							fprintf(outputrs, "用户中断了评测！\n\n");
						}
						else if (tolower(inp) == 'n' || inp == '\n') {
							isexit = false;
						}
						else {
							printf("你的输入有误，请检查后重新输入！\n");
							continue;
						}
						break;
					}
					if (isexit) {
						break;
					}
				}
				#pragma endregion
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
						printferror("数据 %d 输入输出文件 不存在！\n", cnt);
						fprintf(outputrs, "数据 %d | 文件 %s 和 %s 不存在！\n", cnt, filein.c_str(), fileout.c_str());
					}
				}
				else if (checkin == NULL) {
					printferror("数据 %d 输入文件 不存在！\n", cnt);
					fprintf(outputrs, "数据 %d | 文件 %s 不存在！\n", cnt, filein.c_str());
					fclose(checkout);
				}
				else if (checkout == NULL) {
					printferror("数据 %d 输出文件 不存在！\n", cnt);
					fprintf(outputrs, "数据 %d | 文件 %s 不存在！\n", cnt, fileout.c_str());
					fclose(checkin);
				}
				if (checkin == NULL || checkout == NULL) {
					if (endnum != 0) {
						cnt++;
						fprintf(outputrs, "\n");
						continue;
					}
					else {
						break;
					}
				}
				fclose(checkin);
				fclose(checkout);

				STARTUPINFO si;
				PROCESS_INFORMATION programpi;
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
				int ms;
				long long kb;
				int exitcode = 0;
				if (CreateProcess(".\\tmp\\program.exe", NULL, NULL, NULL, !isfreopen, CREATE_SUSPENDED, NULL, ".\\tmp", &si, &programpi)) {
					ResumeThread(programpi.hThread);
					clock_t start = clock();
					int result = WaitForSingleObject(programpi.hProcess, msecond);
					clock_t finish = clock();
					PROCESS_MEMORY_COUNTERS pmc;
					GetProcessMemoryInfo(programpi.hProcess, &pmc, sizeof pmc);
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
					if (GetExitCodeProcess(programpi.hProcess, (DWORD *)&exitcode)) {
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
				if (rs == 2) {
					if (!TerminateProcess(programpi.hProcess, 0)) {
						int code = GetLastError();
						if (code != 6) {
							printferror("无法停止程序一进程，Judge强制停止\n");
							printferror("请自行关闭窗口结束进程\n");
							printferror("错误代码: %d\n", code);
							noticeandexit(1);
						}
					}
				}
				CloseHandle(programpi.hThread);
				CloseHandle(programpi.hProcess);
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
						fprintf(outputrs, "数据 %d | 文件 %s 答案正确 AC 耗时 %d ms 内存 %I64d kB\n", cnt, filein.c_str(), ms, kb);
						AC++;
					}
					else if (status == 2) {
						printfcolor(COLOR_YELLOW, "数据 %d 格式错误 PE", cnt);
						fprintf(outputrs, "数据 %d | 文件 %s 格式错误 PE 耗时 %d ms 内存 %I64d kB\n", cnt, filein.c_str(), ms, kb);
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
						fprintf(outputrs, "数据 %d | 文件 %s 答案错误 WA 耗时 %d ms 内存 %I64d kB\n", cnt, filein.c_str(), ms, kb);
						fprintf(outputrs, "-----\n");
						fprintf(outputrs, "标准输出 stdOutput: \n");
						fprintf(outputrs, "%s\n", backupStdOutput.c_str());
						fprintf(outputrs, "程序输出 programOutput: \n");
						fprintf(outputrs, "%s\n", backupProgramOutput.c_str());
						fprintf(outputrs, "-----\n");
						WA++;
					}

					printf(" 耗时 %d ms 内存 %I64d kB\n", ms, kb);
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
					printfcolor(COLOR_ORGINAL, "数据 %d 内存超限 MLE 内存 %I64d kB\n", cnt, kb);
					fprintf(outputrs, "数据 %d | 文件 %s 内存超限 MLE 内存 %I64d kB\n", cnt, filein.c_str(), kb);
					MLE++;
				}
				fprintf(outputrs, "\n");
				#pragma endregion
				deleteTmpInOutFile();
				Sleep(20);
				cnt++;
			}
		}
		else if (judgemode == 2) {
			SA = TDF = PDF = 0;		
			while (true) {
				#pragma region breakjudge
				if (isexit) {
					printfcolor(COLOR_YELLOW, "你按下了Ctrl + C，确定要中断评测吗？回车默认为不中断\n");
					while (true) {
						setFontColor(COLOR_WHITE);
						printf("breakjudge[y/n]: ");
						rewind(stdin);
						char inp = getchar();
						rewind(stdin);

						setFontColor(COLOR_RED);
						if (tolower(inp) == 'y') {
							printf("用户中断了评测！\n");
							fprintf(outputrs, "用户中断了评测！\n\n");
						}
						else if (tolower(inp) == 'n' || inp == '\n') {
							isexit = false;
						}
						else {
							printf("你的输入有误，请检查后重新输入！\n");
							continue;
						}
						break;
					}
					if (isexit) {
						break;
					}
				}
#pragma endregion
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

				FILE *checkin = fopen(filein.c_str(), "r");

				if (checkin == NULL) {
					if (endnum != 0) {
						printferror("数据 %d 输入文件 不存在！\n", cnt);
						fprintf(outputrs, "数据 %d | 输入文件 %s 不存在！\n", cnt, filein.c_str());
						fprintf(outputrs, "\n");
						cnt++;
						continue;
					}
					else
						break;
				}
				fclose(checkin);

				STARTUPINFO si1, si2;
				PROCESS_INFORMATION programpi1, programpi2;
				HANDLE hIn1 = new HANDLE, hOutRead1 = new HANDLE, hOutWrite1 = new HANDLE;
				HANDLE hIn2 = new HANDLE, hOutRead2 = new HANDLE, hOutWrite2 = new HANDLE;

				string targetinput = ".\\tmp\\";
				if (sourcefirstisfreopen) {
					targetinput += sourcefirstfreopenstdin;
					int returncode = CopyFile(filein.c_str(), targetinput.c_str(), false);
					if (returncode == 0) {
						printferror("复制程序一输入文件时出错！\n");
						printferror("错误代码: %d\n", GetLastError());
						noticeandexit(1);
					}
				}
				targetinput = ".\\tmp\\";
				if (sourcesecondisfreopen) {
					targetinput += sourcesecondfreopenstdin;
					int returncode = CopyFile(filein.c_str(), targetinput.c_str(), false);
					if (returncode == 0) {
						printferror("复制程序二输入文件时出错！\n");
						printferror("错误代码: %d\n", GetLastError());
						noticeandexit(1);
					}
				}

				string output1 = "", output2 = "", backupoutput1 = "", backupoutput2 = "";

				#pragma region run_one
				ZeroMemory(&si1, sizeof STARTUPINFO);
				GetStartupInfo(&si1);
				si1.cb = sizeof STARTUPINFO;
				if (!sourcefirstisfreopen) {
					SECURITY_ATTRIBUTES sa;

					sa.nLength = sizeof SECURITY_ATTRIBUTES;
					sa.lpSecurityDescriptor = NULL;
					sa.bInheritHandle = TRUE;

					hIn1 = CreateFile(filein.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					if (!CreatePipe(&hOutRead1, &hOutWrite1, &sa, 0)) {
						printferror("创建程序一输入管道失败！\n");
						noticeandexit(1);
					}
					si1.hStdInput = hIn1;
					si1.hStdError = hOutWrite1;
					si1.hStdOutput = hOutWrite1;

					si1.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
				}
				else
					si1.dwFlags = STARTF_USESHOWWINDOW;
				si1.wShowWindow = SW_HIDE;
				int rs1 = 0, ms1;
				long long kb1;
				int exitcode1 = 0;
				if (CreateProcess(".\\tmp\\program1.exe", NULL, NULL, NULL, !sourcefirstisfreopen, CREATE_SUSPENDED, NULL, ".\\tmp", &si1, &programpi1)) {
					ResumeThread(programpi1.hThread);
					clock_t start = clock();
					int result = WaitForSingleObject(programpi1.hProcess, msecond);
					clock_t finish = clock();
					PROCESS_MEMORY_COUNTERS pmc;
					GetProcessMemoryInfo(programpi1.hProcess, &pmc, sizeof pmc);
					kb1 = pmc.PeakPagefileUsage / 1024;
					ms1 = (int)(((double)(finish - start) / CLOCKS_PER_SEC) * 1000);
					if (result == WAIT_OBJECT_0) {
						rs1 = 1;
						if (kb1 > memorylimit) rs1 = 4;
					}
					else if (result == WAIT_TIMEOUT)
						rs1 = 2;
					else {
						printferror("Judge程序错误！\n");
						printferror("错误代码: %d\n", GetLastError());
						noticeandexit(1);
					}
					if (GetExitCodeProcess(programpi1.hProcess, (DWORD *)& exitcode1)) {
						if (exitcode1 != 0 && rs1 != 2)
							rs1 = 3;
					}
					else
					{
						printferror("错误！不能读取程序一的返回值！\n");
						printferror("错误代码: %d\n", GetLastError());
						noticeandexit(1);
					}
				}
				else {
					printferror("无法创建被程序一进程，请检查 tmp 目录下被程序一是否存在！\n");
					noticeandexit(1);
				}
				if (!sourcefirstisfreopen) {
					CloseHandle(hIn1);
					CloseHandle(hOutWrite1);
				}
				if (rs1 == 2) {
					if (!TerminateProcess(programpi1.hProcess, 0)) {
						int code = GetLastError();
						if (code != 6) {
							printferror("无法停止程序一进程，Judge强制停止\n");
							printferror("请自行关闭窗口结束进程\n");
							printferror("错误代码: %d\n", code);
							noticeandexit(1);
						}
					}
				}
				CloseHandle(programpi1.hThread);
				CloseHandle(programpi1.hProcess);
				output1 = backupoutput1 = readOutput(hOutRead1, sourcefirstisfreopen, sourcefirstfreopenstdout);
				#pragma endregion
				#pragma region run_two
				ZeroMemory(&si2, sizeof STARTUPINFO);
				GetStartupInfo(&si2);
				si2.cb = sizeof STARTUPINFO;
				if (!sourcesecondisfreopen) {
					SECURITY_ATTRIBUTES sa;

					sa.nLength = sizeof SECURITY_ATTRIBUTES;
					sa.lpSecurityDescriptor = NULL;
					sa.bInheritHandle = TRUE;

					hIn2 = CreateFile(filein.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					if (!CreatePipe(&hOutRead2, &hOutWrite2, &sa, 0)) {
						printferror("创建程序二输入管道失败！\n");
						noticeandexit(1);
					}
					si2.hStdInput = hIn2;
					si2.hStdError = hOutWrite2;
					si2.hStdOutput = hOutWrite2;

					si2.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
				}
				else
					si2.dwFlags = STARTF_USESHOWWINDOW;
				si2.wShowWindow = SW_HIDE;
				int rs2 = 0, ms2;
				long long kb2;
				int exitcode2 = 0;
				if (CreateProcess(".\\tmp\\program2.exe", NULL, NULL, NULL, !sourcesecondisfreopen, CREATE_SUSPENDED, NULL, ".\\tmp", &si2, &programpi2)) {
					ResumeThread(programpi2.hThread);
					clock_t start = clock();
					int result = WaitForSingleObject(programpi2.hProcess, msecond);
					clock_t finish = clock();
					PROCESS_MEMORY_COUNTERS pmc;
					GetProcessMemoryInfo(programpi2.hProcess, &pmc, sizeof pmc);
					kb2 = pmc.PeakPagefileUsage / 1024;
					ms2 = (int)(((double)(finish - start) / CLOCKS_PER_SEC) * 1000);
					if (result == WAIT_OBJECT_0) {
						rs2 = 1;
						if (kb2 > memorylimit) rs2 = 4;
					}
					else if (result == WAIT_TIMEOUT)
						rs2 = 2;
					else {
						printferror("Judge程序错误！\n");
						printferror("错误代码: %d\n", GetLastError());
						noticeandexit(1);
					}
					if (GetExitCodeProcess(programpi2.hProcess, (DWORD *)& exitcode2)) {
						if (exitcode2 != 0 && rs2 != 2)
							rs2 = 3;
					}
					else
					{
						printferror("错误！不能读取程序二的返回值！\n");
						printferror("错误代码: %d\n", GetLastError());
						noticeandexit(1);
					}
				}
				else {
					printferror("无法创建被程序二进程，请检查 tmp 目录下被程序二是否存在！\n");
					noticeandexit(1);
				}
				if (!sourcesecondisfreopen) {
					CloseHandle(hIn2);
					CloseHandle(hOutWrite2);
				}
				if (rs2 == 2) {
					if (!TerminateProcess(programpi2.hProcess, 0)) {
						int code = GetLastError();
						if (code != 6) {
							printferror("无法停止程序一进程，Judge强制停止\n");
							printferror("请自行关闭窗口结束进程\n");
							printferror("错误代码: %d\n", code);
							noticeandexit(1);
						}
					}
				}
				CloseHandle(programpi2.hThread);
				CloseHandle(programpi2.hProcess);
				output2 = backupoutput2 = readOutput(hOutRead2, sourcesecondisfreopen, sourcesecondfreopenstdout);
				#pragma endregion
				#pragma endregion
				if (rs1 == 1 && rs2 == 1) {
					#pragma region compare
														//比对开始
					int status = 0;

					for (int i = 0; i < judgeruleslen; i++) {
						//忽略输出末尾的换行
						if (judgerules[i] == 1) {
							string::iterator it;
							for (it = output1.end() - 1; it >= output1.begin();) {
								if (*it == '\n' || *it == '\r') {
									it = output1.erase(it) - 1;
								}
								else {
									break;
								}
							}
							for (it = output2.end() - 1; it >= output2.begin();) {
								if (*it == '\n' || *it == '\r') {
									it = output2.erase(it) - 1;
								}
								else {
									break;
								}
							}
						}
						//忽略每行前端的空格
						else if (judgerules[i] == 2) {
							string::iterator it;
							for (it = output1.begin(); it != output1.end();) {
								if (*it == '\n') {
									string::iterator it2;
									for (it2 = it + 1; *it2 == ' ';) {
										it2 = output1.erase(it2);
									}
									it = it2;
								}
								else if (*it == '\r') {
									it = output1.erase(it);
								}
								else {
									it++;
								}
							}
							for (it = output1.begin(); it != output1.end();) {
								if (*it == ' ') {
									it = output1.erase(it);
								}
								else {
									break;
								}
							}
							for (it = output2.begin(); it != output2.end();) {
								if (*it == '\n') {
									string::iterator it2;
									for (it2 = it + 1; *it2 == ' ';) {
										it2 = output2.erase(it2);
									}
									it = it2;
								}
								else if (*it == '\r') {
									it = output2.erase(it);
								}
								else {
									it++;
								}
							}
							for (it = output2.begin(); it != output2.end();) {
								if (*it == ' ') {
									it = output2.erase(it);
								}
								else {
									break;
								}
							}
						}
						//忽略每行末尾的空格
						else if (judgerules[i] == 3) {
							string::iterator it;
							for (it = output1.begin(); it != output1.end();) {
								if (*it == '\n') {
									string::iterator it2;
									for (it2 = it - 1; *it2 == ' '; it2--) {
										it2 = output1.erase(it2);
									}
									it = it2 + 2;
								}
								else if (*it == '\r') {
									it = output1.erase(it);
								}
								else {
									it++;
								}
							}
							for (it = output1.end() - 1; it >= output1.begin();) {
								if (*it == ' ') {
									it = output1.erase(it) - 1;
								}
								else {
									break;
								}
							}
							for (it = output2.begin(); it != output2.end();) {
								if (*it == '\n') {
									string::iterator it2;
									for (it2 = it - 1; *it2 == ' '; it2--) {
										it2 = output2.erase(it2);
									}
									it = it2 + 2;
								}
								else if (*it == '\r') {
									it = output2.erase(it);
								}
								else {
									it++;
								}
							}
							for (it = output2.end() - 1; it >= output2.begin();) {
								if (*it == ' ') {
									it = output2.erase(it) - 1;
								}
								else {
									break;
								}
							}
						}
						//忽略重复换行
						else if (judgerules[i] == 4) {
							string::iterator it;
							for (it = output1.begin(); it != output1.end();) {
								if (*it == '\n') {
									string::iterator it2;
									for (it2 = it + 1; *it2 == '\n';) {
										it2 = output1.erase(it2);
									}
									it = it2;
								}
								else if (*it == '\r') {
									it = output1.erase(it);
								}
								else {
									it++;
								}
							}
							for (it = output2.begin(); it != output2.end();) {
								if (*it == '\n') {
									string::iterator it2;
									for (it2 = it + 1; *it2 == '\n';) {
										it2 = output2.erase(it2);
									}
									it = it2;
								}
								else if (*it == '\r') {
									it = output2.erase(it);
								}
								else {
									it++;
								}
							}
						}
						//忽略全部空格
						else if (judgerules[i] == 5) {
							string::iterator it;
							for (it = output1.begin(); it != output1.end();) {
								if (*it == '\n' || *it == '\r') {
									it = output1.erase(it);
								}
								else {
									it++;
								}
							}
							for (it = output2.begin(); it != output2.end();) {
								if (*it == '\n' || *it == '\r') {
									it = output2.erase(it);
								}
								else {
									it++;
								}
							}
						}
						//忽略全部换行
						else if (judgerules[i] == 6) {
							string::iterator it;
							for (it = output1.begin(); it != output1.end();) {
								if (*it == ' ' || *it == '\r') {
									it = output1.erase(it);
								}
								else {
									it++;
								}
							}
							for (it = output2.begin(); it != output2.end();) {
								if (*it == ' ' || *it == '\r') {
									it = output2.erase(it);
								}
								else {
									it++;
								}
							}
						}
					}

					if (output1 == output2) {
						status = 1;  // 1 为 SA
					}
					else {
						string::iterator it;
						for (it = output1.begin(); it != output1.end();) {
							if (*it == '\n' || *it == '\r' || *it == ' ') {
								it = output1.erase(it);
							}
							else {
								it++;
							}
						}
						for (it = output2.begin(); it != output2.end();) {
							if (*it == '\n' || *it == '\r' || *it == ' ') {
								it = output2.erase(it);
							}
							else {
								it++;
							}
						}
						if (output1 == output2) {
							status = 2; //2 为 PDF
						}
						else {
							status = 3; //3 为 TDF
						}
					}
#pragma endregion
					#pragma region report
					printf("数据 %d ", cnt);
					fprintf(outputrs, "数据 %d | 文件 %s ", cnt, filein.c_str());
					if (status == 1) {
						printfcolor(COLOR_GREEN, "输出相同 SA ");
						fprintf(outputrs, "输出相同 SA ");
						SA++;
					}
					else if (status == 2) {
						printfcolor(COLOR_YELLOW, "格式不同 PDF ");
						fprintf(outputrs, "格式不同 PDF ");
						fprintf(outputrs, "-----\n");
						fprintf(outputrs, "程序一输出 programOutput_1: \n");
						fprintf(outputrs, "%s\n", backupoutput1.c_str());
						fprintf(outputrs, "程序二输出 programOutput_2: \n");
						fprintf(outputrs, "%s\n", backupoutput2.c_str());
						fprintf(outputrs, "-----\n");
						PDF++;
					}
					else if (status == 3) {
						printfcolor(COLOR_RED, "输出完全不同 TDF ");
						fprintf(outputrs, "输出完全不同 TDF ");
						fprintf(outputrs, "-----\n");
						fprintf(outputrs, "程序一输出 programOutput_1: \n");
						fprintf(outputrs, "%s\n", backupoutput1.c_str());
						fprintf(outputrs, "程序二输出 programOutput_2: \n");
						fprintf(outputrs, "%s\n", backupoutput2.c_str());
						fprintf(outputrs, "-----\n");
						TDF++;
					}

					printf("程序一 耗时 %d ms 内存 %I64d kB 程序二 耗时 %d ms 内存 %I64d kB\n", ms1, kb1, ms2, kb2);
					fprintf(outputrs, "程序一 耗时 %d ms 内存 %I64d kB 程序二 耗时 %d ms 内存 %I64d kB\n", ms1, kb1, ms2, kb2);
				#pragma endregion
				}
				#pragma region reportother
				else {
					printf("数据 %d:\n", cnt);
					fprintf(outputrs, "数据 %d | 文件 %s ", cnt, filein.c_str());
					if (rs1 == 2) {
						printfcolor(COLOR_BLUE, "程序一 时间超限 TLE ");
						fprintf(outputrs, "程序一 时间超限 TLE ");
					}
					else if (rs1 == 3) {
						printfcolor(COLOR_CYAN, "程序一 运行错误 RE ");
						fprintf(outputrs, "程序一 运行错误 RE ");
					}
					else if (rs1 == 4) {
						printfcolor(COLOR_ORGINAL, "程序一 内存超限 MLE 内存 %I64d kB ", kb1);
						fprintf(outputrs, "程序一 内存超限 MLE 内存 %I64d kB ", kb1);
					}
					else if (rs1 == 1) {
						printfcolor(COLOR_GREEN, "程序一 运行正常 NORMAL 时间 %d ms 内存 %I64d kB ", ms1, kb1);
						fprintf(outputrs, "程序一 运行正常 NORMAL 时间 %d ms 内存 %I64d kB ", ms1, kb1);
					}
					if (rs2 == 2) {
						printfcolor(COLOR_BLUE, "程序二 时间超限 TLE\n");
						fprintf(outputrs, "程序二 时间超限 TLE\n");
					}
					else if (rs2 == 3) {
						printfcolor(COLOR_CYAN, "程序二 运行错误 RE\n");
						fprintf(outputrs, "程序二 运行错误 RE\n");
					}
					else if (rs2 == 4) {
						printfcolor(COLOR_ORGINAL, "程序二 内存超限 MLE 内存 %I64d kB\n", kb2);
						fprintf(outputrs, "程序二 内存超限 MLE 内存 %I64d kB\n", kb2);
					}
					else if (rs2 == 1) {
						printfcolor(COLOR_GREEN, "程序二 运行正常 NORMAL 时间 %d ms 内存 %I64d kB\n", ms2, kb2);
						fprintf(outputrs, "程序二 运行正常 NORMAL 时间 %d ms 内存 %I64d kB\n", ms2, kb2);
					}
					TDF++;
				}
				fprintf(outputrs, "\n");
				#pragma endregion
				deleteTmpInOutFile();
				Sleep(20);
				cnt++;
			}
		}
		SetConsoleCtrlHandler((PHANDLER_ROUTINE)InJudgeCtrlCHandle, false);
	#pragma endregion
	#pragma region afterall
		/*
		完成后
		*/
		SetConsoleCtrlHandler((PHANDLER_ROUTINE)OutJudgeCtrlCHandle, true);
		printf("\n评测结束\n");
		printf("\n");
		if (judgemode == 1) {
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
		}
		else if (judgemode == 2) {
			if (SA != 0) {
				printfcolor(COLOR_GREEN, "输出相同 SA: %d\n", SA);
				fprintf(outputrs, "答输出相同 SA: %d\n", SA);
			}
			if (TDF != 0) {
				printfcolor(COLOR_RED, "输出完全不同 TDF: %d\n", TDF);
				fprintf(outputrs, "答案错误 WA: %d\n", TDF);
			}
			if (PDF != 0) {
				printfcolor(COLOR_YELLOW, "格式不同 PDF: %d\n", PDF);
				fprintf(outputrs, "格式不同 PDF: %d\n", PDF);
			}
		}
		fclose(outputrs);
		printf("\n");
		printfcolor(COLOR_GREEN, "评测结束！\n");

		deleteTmpProgram();
		deleteTmpDir();

		fcloseall();
	#pragma endregion
	#pragma region exit
		pressAnyButtonTo("退出");
		SetConsoleCtrlHandler((PHANDLER_ROUTINE)OutJudgeCtrlCHandle, false);
		exit(0);
	#pragma endregion
	}
#pragma endregion
}
