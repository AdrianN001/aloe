#ifndef __MODE_H_
#define __MODE_H_

#ifndef CTRL
#define CTRL(c) ((c) & 037)
#endif

#define BASE_MODE           0
#define TEXT_EDITOR_MODE    1
#define TERMINAL_MODE       2
#define WORKSPACE_MODE      3

#define FILE_SEARCH_MODE    4



#define BASE_MODE_KEY           27  // esc
#define TEXT_EDITOR_MODE_KEY    ((int)'a')  
#define TERMINAL_MODE_KEY       ((int)'t') 
#define WORKSPACE_MODE_KEY      ((int)'w')
#define FILE_SEARCH_MODE_KEY    ((int)'s')


#endif