" vi:syntax=vim

" simple-blue
" by Davide Stasio
" NOTE: gui_cyan and gui_yellow used for unset values
hi clear
syntax reset
let g:colors_name = "simple-blue"

" GUI colors
let g:shi_bg       = "004B6A"
let g:shi_bg_alt   = "003D57"
"let g:shi_bg_alt   = \"00577A"
let g:shi_fg       = "C5C8C6"
let g:shi_fg_alt   = "919391"
let g:shi_accent   = "282A2E"
let g:shi_comment  = "00CE8A"
let g:shi_keyword  = "00D3BB"
let g:shi_str      = "D8B300"
let g:shi_str_alt  = "BB9B00"
let g:shi_number   = "0AC06D"
let g:shi_preproc  = "DA6B87"
let g:shi_cursor   = g:shi_accent
let g:shi_error    = g:shi_preproc

let g:shi_err_yellow   = "FFFF00"
let g:shi_err_cyan     = "00FFFF"
let g:shi_err_magenta  = "FF00FF"
let g:shi_err_red      = "FF0000"
let g:shi_err_green    = "00FF00"
let g:shi_err_blue     = "0000FF"

"func highlight()
ru colors/simple-template.vim

" Remove color variables
unlet g:shi_bg g:shi_bg_alt g:shi_fg g:shi_fg_alt g:shi_accent g:shi_comment g:shi_keyword g:shi_str g:shi_str_alt g:shi_number g:shi_preproc g:shi_cursor g:shi_error
unlet g:shi_err_yellow g:shi_err_cyan g:shi_err_magenta g:shi_err_red g:shi_err_green g:shi_err_blue
