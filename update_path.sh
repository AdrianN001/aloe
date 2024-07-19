EXECUTABLE_FILE=alo3
CSTM_BIN_DIR=~/cstm_bin

function copy_new_file_to_cstm_bin() {
    if [ -f "$CSTM_BIN_DIR/$EXECUTABLE_FILE" ]; then 
        rename_old_file
        move_new_binary_to_directory
    else 
        move_new_binary_to_directory
    fi
}

function rename_old_file() {
    mv "$CSTM_BIN_DIR/$EXECUTABLE_FILE" "$CSTM_BIN_DIR/$EXECUTABLE_FILE.old"
}

function move_new_binary_to_directory() { 
    cp "$EXECUTABLE_FILE" "$CSTM_BIN_DIR/$EXECUTABLE_FILE" 
}

copy_new_file_to_cstm_bin
echo "Path Updated"