fun s:highlight(group, guifg, guibg, ctermfg, ctermbg, attr, guisp)
  if a:guifg != ""
    exec "hi " . a:group . " guifg=#" . a:guifg
  endif
  if a:guibg != ""
    exec "hi " . a:group . " guibg=#" . a:guibg
  endif
  if a:ctermfg != ""
    exec "hi " . a:group . " ctermfg=" . a:ctermfg
  endif
  if a:ctermbg != ""
    exec "hi " . a:group . " ctermbg=" . a:ctermbg
  endif
  if a:attr != ""
    exec "hi " . a:group . " gui=" . a:attr . " cterm=" . a:attr
  endif
  if a:guisp != ""
    exec "hi " . a:group . " guisp=#" . a:guisp
  endif
endfun

" Vim editor colors
call s:highlight("Normal",       g:shi_fg, g:shi_bg, "", "", "", "")
call s:highlight("Bold",         "", "", "", "", "bold", "")
call s:highlight("Debug",        g:shi_err_cyan, "", "", "", "", "")
call s:highlight("Directory",    g:shi_err_cyan, "", "", "", "", "")
call s:highlight("Error",        g:shi_accent, g:shi_error, "", "", "", "")
call s:highlight("ErrorMsg",     g:shi_accent, g:shi_error, "", "", "", "")
call s:highlight("Exception",    g:shi_err_magenta, "", "", "", "", "")
call s:highlight("FoldColumn",   g:shi_fg_alt, g:shi_bg, "", "", "", "")
call s:highlight("Folded",       g:shi_fg_alt, g:shi_bg_alt, "", "", "", "")
call s:highlight("IncSearch",    g:shi_err_cyan, g:shi_bg_alt, "", "", "none", "")
call s:highlight("Italic",       "", "", "", "", "none", "")
call s:highlight("Macro",        g:shi_preproc, "", "", "", "", "")
call s:highlight("MatchParen",   "", g:shi_accent, "", "",  "", "")
call s:highlight("ModeMsg",      g:shi_str, "", "", "", "", "")
call s:highlight("MoreMsg",      g:shi_str, "", "", "", "", "")
call s:highlight("Question",     g:shi_fg, "", "", "", "", "")
call s:highlight("Search",       g:shi_fg, g:shi_accent, "", "",  "", "")
call s:highlight("Substitute",   g:shi_fg, g:shi_accent, "", "", "none", "")
call s:highlight("SpecialKey",   g:shi_fg, "", "", "", "", "")
call s:highlight("TooLong",      g:shi_err_magenta, "", "", "", "", "")
call s:highlight("Underlined",   g:shi_keyword, "", "", "", "", "")
call s:highlight("Visual",       "", g:shi_accent, "", "", "", "")
call s:highlight("VisualNOS",    g:shi_err_magenta, "", "", "", "", "")
call s:highlight("WarningMsg",   g:shi_error, "", "", "", "", "")
call s:highlight("WildMenu",     g:shi_fg, g:shi_accent, "", "", "", "")
call s:highlight("Title",        g:shi_fg, "", "", "", "none", "")
call s:highlight("Conceal",      g:shi_fg_alt, g:shi_err_yellow, "", "", "", "")
call s:highlight("Cursor",       g:shi_bg, g:shi_cursor, "", "", "", "")
call s:highlight("NonText",      g:shi_cursor, "", "", "", "", "")
call s:highlight("LineNr",       g:shi_fg, g:shi_bg, "", "", "", "")
call s:highlight("SignColumn",   g:shi_err_cyan, g:shi_err_yellow, "", "", "", "")
call s:highlight("StatusLine",   g:shi_fg_alt, g:shi_accent, "", "", "none", "")
call s:highlight("StatusLineNC", g:shi_fg_alt, g:shi_bg_alt, "", "", "none", "")
call s:highlight("VertSplit",    g:shi_accent, g:shi_accent, "", "", "none", "")
call s:highlight("ColorColumn",  "", g:shi_bg_alt, "", "", "none", "")
call s:highlight("CursorColumn", "", g:shi_bg_alt, "", "", "none", "")
call s:highlight("CursorLine",   "", g:shi_bg_alt, "", "", "none", "")
call s:highlight("CursorLineNr", g:shi_fg, g:shi_bg, "", "", "", "")
call s:highlight("QuickFixLine", "", g:shi_bg_alt, "", "", "none", "")
call s:highlight("PMenu",        g:shi_fg_alt, g:shi_bg, "", "", "none", "")
call s:highlight("PMenuSel",     g:shi_fg, g:shi_bg_alt, "", "", "", "")
call s:highlight("TabLine",      g:shi_err_cyan, g:shi_err_yellow, "", "", "none", "")
call s:highlight("TabLineFill",  g:shi_err_cyan, g:shi_err_yellow, "", "", "none", "")
call s:highlight("TabLineSel",   g:shi_err_cyan, g:shi_err_yellow, "", "", "none", "")

" Standard syntax s:highlighting
call s:highlight("Boolean",      g:shi_keyword, "", "", "", "", "")
call s:highlight("Character",    g:shi_str, "", "", "", "", "")
call s:highlight("Comment",      g:shi_comment, "", "", "", "", "")
call s:highlight("Conditional",  g:shi_keyword, "", "", "", "", "")
call s:highlight("Constant",     "", "", "", "", "", "")
call s:highlight("Define",       g:shi_preproc, "", "", "", "none", "")
call s:highlight("Delimiter",    g:shi_fg, "", "", "", "", "")
call s:highlight("Float",        g:shi_number, "", "", "", "", "")
call s:highlight("Function",     g:shi_fg, "", "", "", "", "")
call s:highlight("Identifier",   g:shi_fg, "", "", "", "none", "")
call s:highlight("Include",      g:shi_preproc, "", "", "", "", "")
call s:highlight("Keyword",      g:shi_keyword, "", "", "", "", "")
call s:highlight("Label",        g:shi_keyword, "", "", "", "", "")
call s:highlight("Number",       g:shi_number, "", "", "", "", "")
call s:highlight("Operator",     g:shi_fg, "", "", "", "none", "")
call s:highlight("PreProc",      g:shi_accent, "", "", "", "", "")
call s:highlight("Repeat",       g:shi_keyword, "", "", "", "", "")
call s:highlight("Special",      g:shi_keyword, "", "", "", "", "")
call s:highlight("SpecialChar",  g:shi_str_alt, "", "", "", "", "")
call s:highlight("Statement",    g:shi_keyword, "", "", "", "", "")
call s:highlight("StorageClass", g:shi_keyword, "", "", "", "", "")
call s:highlight("String",       g:shi_str, "", "", "", "", "")
call s:highlight("Structure",    g:shi_keyword, "", "", "", "", "")
call s:highlight("Tag",          g:shi_str, "", "", "", "", "")
call s:highlight("Todo",         g:shi_str, g:shi_accent, "", "", "", "")
call s:highlight("Type",         g:shi_keyword, "", "", "", "none", "")
call s:highlight("Typedef",      g:shi_err_magenta, "", "", "", "", "")

" C s:highlighting
call s:highlight("cOperator",   g:shi_keyword, "", "", "", "", "")
call s:highlight("cPreCondit",  g:shi_preproc, "", "", "", "", "")

" C# s:highlighting
call s:highlight("csClass",                 g:shi_err_cyan, "", "", "", "", "")
call s:highlight("csAttribute",             g:shi_err_cyan, "", "", "", "", "")
call s:highlight("csModifier",              g:shi_err_cyan, "", "", "", "", "")
call s:highlight("csType",                  g:shi_err_cyan, "", "", "", "", "")
call s:highlight("csUnspecifiedStatement",  g:shi_err_cyan, "", "", "", "", "")
call s:highlight("csContextualStatement",   g:shi_err_cyan, "", "", "", "", "")
call s:highlight("csNewDecleration",        g:shi_err_cyan, "", "", "", "", "")

" CSS s:highlighting
call s:highlight("cssBraces",      g:shi_err_cyan, "", "", "", "", "")
call s:highlight("cssClassName",   g:shi_err_cyan, "", "", "", "", "")
call s:highlight("cssColor",       g:shi_err_cyan, "", "", "", "", "")

" Diff s:highlighting
call s:highlight("DiffAdd",      g:shi_err_cyan, g:shi_err_yellow,  "", "", "", "")
call s:highlight("DiffChange",   g:shi_err_cyan, g:shi_err_yellow,  "", "", "", "")
call s:highlight("DiffDelete",   g:shi_err_cyan, g:shi_err_yellow,  "", "", "", "")
call s:highlight("DiffText",     g:shi_err_cyan, g:shi_err_yellow,  "", "", "", "")
call s:highlight("DiffAdded",    g:shi_err_cyan, g:shi_err_yellow,  "", "", "", "")
call s:highlight("DiffFile",     g:shi_err_cyan, g:shi_err_yellow,  "", "", "", "")
call s:highlight("DiffNewFile",  g:shi_err_cyan, g:shi_err_yellow,  "", "", "", "")
call s:highlight("DiffLine",     g:shi_err_cyan, g:shi_err_yellow,  "", "", "", "")
call s:highlight("DiffRemoved",  g:shi_err_cyan, g:shi_err_yellow,  "", "", "", "")

" Git s:highlighting
call s:highlight("gitcommitOverflow",       g:shi_err_cyan, "", "", "", "", "")
call s:highlight("gitcommitSummary",        g:shi_err_cyan, "", "", "", "", "")
call s:highlight("gitcommitComment",        g:shi_err_cyan, "", "", "", "", "")
call s:highlight("gitcommitUntracked",      g:shi_err_cyan, "", "", "", "", "")
call s:highlight("gitcommitDiscarded",      g:shi_err_cyan, "", "", "", "", "")
call s:highlight("gitcommitSelected",       g:shi_err_cyan, "", "", "", "", "")
call s:highlight("gitcommitHeader",         g:shi_err_cyan, "", "", "", "", "")
call s:highlight("gitcommitSelectedType",   g:shi_err_cyan, "", "", "", "", "")
call s:highlight("gitcommitUnmergedType",   g:shi_err_cyan, "", "", "", "", "")
call s:highlight("gitcommitDiscardedType",  g:shi_err_cyan, "", "", "", "", "")
call s:highlight("gitcommitBranch",         g:shi_err_cyan, "", "", "", "bold", "")
call s:highlight("gitcommitUntrackedFile",  g:shi_err_cyan, "", "", "", "", "")
call s:highlight("gitcommitUnmergedFile",   g:shi_err_cyan, "", "", "", "bold", "")
call s:highlight("gitcommitDiscardedFile",  g:shi_err_cyan, "", "", "", "bold", "")
call s:highlight("gitcommitSelectedFile",   g:shi_err_cyan, "", "", "", "bold", "")

" GitGutter s:highlighting
call s:highlight("GitGutterAdd",           g:shi_err_cyan, g:shi_err_yellow, "", "", "", "")
call s:highlight("GitGutterChange",        g:shi_err_cyan, g:shi_err_yellow, "", "", "", "")
call s:highlight("GitGutterDelete",        g:shi_err_cyan, g:shi_err_yellow, "", "", "", "")
call s:highlight("GitGutterChangeDelete",  g:shi_err_cyan, g:shi_err_yellow, "", "", "", "")

" HTML s:highlighting
call s:highlight("htmlBold",    g:shi_err_cyan, "", "", "", "", "")
call s:highlight("htmlItalic",  g:shi_err_cyan, "", "", "", "", "")
call s:highlight("htmlEndTag",  g:shi_err_cyan, "", "", "", "", "")
call s:highlight("htmlTag",     g:shi_err_cyan, "", "", "", "", "")

" JavaScript s:highlighting
call s:highlight("javaScript",        g:shi_err_cyan, "", "", "", "", "")
call s:highlight("javaScriptBraces",  g:shi_err_cyan, "", "", "", "", "")
call s:highlight("javaScriptNumber",  g:shi_err_cyan, "", "", "", "", "")
" pangloss/vim-javascript s:highlighting
call s:highlight("jsOperator",          g:shi_err_cyan, "", "", "", "", "")
call s:highlight("jsStatement",         g:shi_err_cyan, "", "", "", "", "")
call s:highlight("jsReturn",            g:shi_err_cyan, "", "", "", "", "")
call s:highlight("jsThis",              g:shi_err_cyan, "", "", "", "", "")
call s:highlight("jsClassDefinition",   g:shi_err_cyan, "", "", "", "", "")
call s:highlight("jsFunction",          g:shi_err_cyan, "", "", "", "", "")
call s:highlight("jsFuncName",          g:shi_err_cyan, "", "", "", "", "")
call s:highlight("jsFuncCall",          g:shi_err_cyan, "", "", "", "", "")
call s:highlight("jsClassFuncName",     g:shi_err_cyan, "", "", "", "", "")
call s:highlight("jsClassMethodType",   g:shi_err_cyan, "", "", "", "", "")
call s:highlight("jsRegexpString",      g:shi_err_cyan, "", "", "", "", "")
call s:highlight("jsGlobalObjects",     g:shi_err_cyan, "", "", "", "", "")
call s:highlight("jsGlobalNodeObjects", g:shi_err_cyan, "", "", "", "", "")
call s:highlight("jsExceptions",        g:shi_err_cyan, "", "", "", "", "")
call s:highlight("jsBuiltins",          g:shi_err_cyan, "", "", "", "", "")

" Mail s:highlighting
call s:highlight("mailQuoted1",  g:shi_err_cyan, "", "", "", "", "")
call s:highlight("mailQuoted2",  g:shi_err_cyan, "", "", "", "", "")
call s:highlight("mailQuoted3",  g:shi_err_cyan, "", "", "", "", "")
call s:highlight("mailQuoted4",  g:shi_err_cyan, "", "", "", "", "")
call s:highlight("mailQuoted5",  g:shi_err_cyan, "", "", "", "", "")
call s:highlight("mailQuoted6",  g:shi_err_cyan, "", "", "", "", "")
call s:highlight("mailURL",      g:shi_err_cyan, "", "", "", "", "")
call s:highlight("mailEmail",    g:shi_err_cyan, "", "", "", "", "")

" Markdown s:highlighting
call s:highlight("markdownCode",              g:shi_err_cyan, "", "", "", "", "")
call s:highlight("markdownError",             g:shi_err_cyan, g:shi_err_yellow, "", "", "", "")
call s:highlight("markdownCodeBlock",         g:shi_err_cyan, "", "", "", "", "")
call s:highlight("markdownHeadingDelimiter",  g:shi_err_cyan, "", "", "", "", "")

" NERDTree s:highlighting
call s:highlight("NERDTreeDirSlash",  g:shi_err_cyan, "", "", "", "", "")
call s:highlight("NERDTreeExecFile",  g:shi_err_cyan, "", "", "", "", "")

" PHP s:highlighting
call s:highlight("phpMemberSelector",  g:shi_err_cyan, "", "", "", "", "")
call s:highlight("phpComparison",      g:shi_err_cyan, "", "", "", "", "")
call s:highlight("phpParent",          g:shi_err_cyan, "", "", "", "", "")
call s:highlight("phpMethodsVar",      g:shi_err_cyan, "", "", "", "", "")

" Python s:highlighting
call s:highlight("pythonOperator",  g:shi_err_cyan, "", "", "", "", "")
call s:highlight("pythonRepeat",    g:shi_err_cyan, "", "", "", "", "")
call s:highlight("pythonInclude",   g:shi_err_cyan, "", "", "", "", "")
call s:highlight("pythonStatement", g:shi_err_cyan, "", "", "", "", "")

" Ruby s:highlighting
call s:highlight("rubyAttribute",               g:shi_err_cyan, "", "", "", "", "")
call s:highlight("rubyConstant",                g:shi_err_cyan, "", "", "", "", "")
call s:highlight("rubyInterpolationDelimiter",  g:shi_err_cyan, "", "", "", "", "")
call s:highlight("rubyRegexp",                  g:shi_err_cyan, "", "", "", "", "")
call s:highlight("rubySymbol",                  g:shi_err_cyan, "", "", "", "", "")
call s:highlight("rubyStringDelimiter",         g:shi_err_cyan, "", "", "", "", "")

" SASS s:highlighting
call s:highlight("sassidChar",     g:shi_err_cyan, "", "", "", "", "")
call s:highlight("sassClassChar",  g:shi_err_cyan, "", "", "", "", "")
call s:highlight("sassInclude",    g:shi_err_cyan, "", "", "", "", "")
call s:highlight("sassMixing",     g:shi_err_cyan, "", "", "", "", "")
call s:highlight("sassMixinName",  g:shi_err_cyan, "", "", "", "", "")

" Signify s:highlighting
call s:highlight("SignifySignAdd",     g:shi_err_cyan, g:shi_err_yellow, "", "", "", "")
call s:highlight("SignifySignChange",  g:shi_err_cyan, g:shi_err_yellow, "", "", "", "")
call s:highlight("SignifySignDelete",  g:shi_err_cyan, g:shi_err_yellow, "", "", "", "")

" Spelling s:highlighting
call s:highlight("SpellBad",     "", "", "", "", "undercurl", g:shi_err_cyan)
call s:highlight("SpellLocal",   "", "", "", "", "undercurl", g:shi_err_cyan)
call s:highlight("SpellCap",     "", "", "", "", "undercurl", g:shi_err_cyan)
call s:highlight("SpellRare",    "", "", "", "", "undercurl", g:shi_err_cyan)

" Startify s:highlighting
call s:highlight("StartifyBracket",  g:shi_err_cyan, "", "", "", "", "")
call s:highlight("StartifyFile",     g:shi_err_cyan, "", "", "", "", "")
call s:highlight("StartifyFooter",   g:shi_err_cyan, "", "", "", "", "")
call s:highlight("StartifyHeader",   g:shi_err_cyan, "", "", "", "", "")
call s:highlight("StartifyNumber",   g:shi_err_cyan, "", "", "", "", "")
call s:highlight("StartifyPath",     g:shi_err_cyan, "", "", "", "", "")
call s:highlight("StartifySection",  g:shi_err_cyan, "", "", "", "", "")
call s:highlight("StartifySelect",   g:shi_err_cyan, "", "", "", "", "")
call s:highlight("StartifySlash",    g:shi_err_cyan, "", "", "", "", "")
call s:highlight("StartifySpecial",  g:shi_err_cyan, "", "", "", "", "")

" Java s:highlighting
call s:highlight("javaOperator",     g:shi_err_cyan, "", "", "", "", "")

" Remove functions
delf s:highlight


