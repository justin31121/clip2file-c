#include <stdio.h>

#include <windows.h>

char * error_cstr(DWORD error_code) {
    char * messageBuffer = NULL;

    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
    
    return messageBuffer;
}

int main(int argc, char **argv) {

    if(argc < 2) {
	fprintf(stderr, "ERROR: Please provide output-path\n");
	fprintf(stderr, "USAGE: %s <output-path>\n", argv[0]);
	return 1;
    }

    const char *output = argv[1];

    //GET CLIPBOARD TEXT
    if (!OpenClipboard(NULL)) {
	int error = GetLastError();
	fprintf(stderr, "ERROR: Failed to open clipboard: (%d) %s",
		error, error_cstr(error));
	return 1;
    }

    HANDLE handle = GetClipboardData(CF_TEXT);
    if(handle == NULL) {
	int error = GetLastError();
	fprintf(stderr, "ERROR: Failed to get clipboard content: (%d) %s",
		error, error_cstr(error));
	return 1;
    }

    char *content = GlobalLock(handle);
    if(content == NULL) {
	int error = GetLastError();
	fprintf(stderr, "ERROR: Failed to copy clipboard content: (%d) %s",
		error, error_cstr(error));
	return 1;
    }  

    //GlobalUnlock(handle);
    //CloseClipboard();

    //WRITE THE FILE
    HANDLE file_handle = CreateFile(output,
			       GENERIC_WRITE, 0, NULL,
			       CREATE_ALWAYS,
			       FILE_ATTRIBUTE_NORMAL,
			       NULL);
    if(file_handle == INVALID_HANDLE_VALUE) {
	int error = GetLastError();
	fprintf(stderr, "ERROR: Failed to open '%s': (%d) %s", output,
		error, error_cstr(error));
	return 1;
    }    

    DWORD to_write = (DWORD) strlen(content);
    DWORD written;
    if(!WriteFile(file_handle, content, to_write, &written, NULL) ||
	written != to_write) {
	int error = GetLastError();
	fprintf(stderr, "ERROR: Failed to write to file '%s': (%d) %s", output,
		error, error_cstr(error));
	return 1;
    }

    printf("Written clipboard content: '%s'\n", output);

    //CloseHandle(file_handle);
    
    return 0;
}
