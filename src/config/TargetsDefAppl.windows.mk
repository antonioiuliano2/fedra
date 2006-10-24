#------------------------------------------------------------------------------
CPPFLAGS     = $(CPPFLAGS) -I$(INC_DIR)
#CXXFLAGS    = $(CPPFLAGS) -I$(INC_DIR)
CFLAGS       = $(CPPFLAGS) -I$(INC_DIR) /TP

#OBJ       = $(NAME:.=.obj)

#$(NAME): $(OBJ)

$(NAME).exe : $(NAME).obj
        $(LD) $**  $(LIBS) $(PROJECT_LIBS:-l=/DEFAULTLIB:lib) -out:$(BIN_DIR)/$@

#------------------------------------------------------------------------------
clean:
    @for %i in ($(NAME)) DO    FOR %F IN ( $(NAME).obj "$(BIN_DIR)"\$(NAME).exe ) DO IF EXIST %F del /q %F