#include "aloe/fs.h" 
#include "aloe/assert.h" 
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <pwd.h>

#define add_matching(file_extension, match_to_file_extension, file_type)  if (strcmp(file_extension, match_to_file_extension) == 0) return file_type

char* format_file_size(long long bytes, char* out){
    double size = bytes;
    char* unit;
    
    if (size < 1024) {
        unit = "bytes";
    } else if (size < 1024 * 1024) {
        size /= 1024;
        unit = "KB";
    } else if (size < 1024 * 1024 * 1024) {
        size /= (1024 * 1024);
        unit = "MB";
    } else {
        size /= (1024 * 1024 * 1024);
        unit = "GB";
    }
    
    sprintf(out, "%.2f %s", size, unit);
    
    return out;
}

char* get_owner(struct stat* info){
    struct passwd *pw = getpwuid(info->st_uid);
    return pw->pw_name;
}

file_info_t get_file_metadata(file_t* file){
    FILE* fp = file->fp;
    struct stat file_stat;

    int res = fstat(fileno(fp),&file_stat);
    assert_with_log_s(res == 0, "could't fetch the metadata of a file");

    long long bytes_size = file_stat.st_size;
    char* size_buffer = malloc(sizeof(char) * 20);
    format_file_size(bytes_size, size_buffer);

    char* last_time_accessed = ctime(&file_stat.st_atime);
    char* last_time_modified = ctime(&file_stat.st_mtime);
    
    char* file_type = match_file_extension_with_file_type(file->file_name);
    char* file_owner = get_owner(&file_stat);

    return (file_info_t){
        .file_name = file->file_name,
        .file_size = size_buffer,
        .file_type = file_type,
        .file_owner = file_owner,
        .last_time_modified = last_time_modified,
        .last_time_accessed = last_time_accessed
    };
}  


char* get_file_extension_from_path(char* file_path){
    char *extension = strrchr(file_path, '.'); // Find the last occurrence of '.' in the file path
    if (extension == NULL) { 
        return NULL; 
    } 
    return extension; // Return the file extension
}

char* match_file_extension_with_file_type(char* file_name){
    add_matching(file_name, "Makefile", "Make file");
    add_matching(file_name, "CMakeLists.txt", "Cmake file");
    
    char* file_extension = get_file_extension_from_path(file_name);
    if (file_extension == NULL){
        return "Plain text file";
    }

    add_matching(file_extension,".h", "C/C++ header file");

    add_matching(file_extension,".c", "C source file");

    add_matching(file_extension,".cs", "C# source file");
    add_matching(file_extension,".java", "Java source file");

    add_matching(file_extension,".cpp", "C++ source file");
    add_matching(file_extension,".hpp", "C++ header file");

    add_matching(file_extension,".py", "Python source file");
    add_matching(file_extension,".js", "JavaScript source file");
    add_matching(file_extension,".ts", "TypeScript source file");
    add_matching(file_extension, ".php", "PHP source file");
    add_matching(file_extension, ".jsx", "JSX file");
    add_matching(file_extension, ".tsx", "Typed JSX file");


    add_matching(file_extension,".go", "Golang source file");

    add_matching(file_extension,".txt", "Plain text file");

    add_matching(file_extension, ".dockerfile", "Dockerfile");
    add_matching(file_extension, ".json", "JSON file");

    add_matching(file_extension, ".html", "HTML markup file");
    add_matching(file_extension, ".css", "CSS source file");



    return "Unknown file type";
}

void free_file_metadata(file_info_t* file_info){
    free(file_info->file_size);
}

#undef add_matching


