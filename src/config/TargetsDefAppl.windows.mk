#------------------------------------------------------------------------------
CPPFLAGS     = $(CPPFLAGS) -I$(INC_DIR)

#OBJ       = $(NAME:.=.obj)

#$(NAME): $(OBJ)

$(NAME).exe : $(NAME).obj
        $(LD) $**  $(LIBS) $(PROJECT_LIBS:-l=/DEFAULTLIB:lib) -out:$(BIN_DIR)/$@

#------------------------------------------------------------------------------
clean:
    @for %f in ($(NAME)) DO del /q $(NAME).obj "$(BIN_DIR)"\$(NAME).exe