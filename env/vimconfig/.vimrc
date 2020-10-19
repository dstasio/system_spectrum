" TODO: syntax highlighting for NOTEs, TODOs and IMPORTANTs
" TODO: syntax highlighting for WARNING in quickfix
" TODO: reorder .vimrc
" TODO: maybe implement file backup
" TODO: look at conceal characters
" TODO: ctrl-x to switch to last unrelated file (different than what you'd get with ctrl-c)
syntax on
colorscheme base16-gruvbox-dark-pale
set number
set wrap!
set textwidth=0
set relativenumber
set wildmode=list:full
set wildmenu
set errorformat+=%f(%l\\,%c):\ %t%*\\D%n:\ %m
set incsearch

nnoremap <silent> <A-Space> :set hlsearch! <Bar>:echo<CR>

" Highlighting 70th column
"if (exists('+colorcolumn'))
"    set colorcolumn=70
"    highlight ColorColumn ctermbg=9
"endif

" Setting fonts for gvim (others will be added)
if has("gui_running")
  if has("gui_gtk2")
    set guifont=Inconsolata\ 12
  elseif has("gui_macvim")
    set guifont=Menlo\ Regular:h14
  elseif has("gui_win32")
    set guifont=Consolas:h11:cANSI
    set guioptions-=m
    set guioptions-=T
    set guioptions-=r
    set guioptions-=L
  endif
endif

function! SplitOnce()
    simalt ~x
    if exists("w:IsSplit") == 0
        vsplit
        let w:IsSplit = 1
    endif
endfun

" Split window on open (splits twice in gvim)
au GUIEnter * call SplitOnce()
au VimResized * winc =
au BufNewFile,BufRead *.hlsl set syntax=hlsl

nnoremap <silent> <A-w> :set wrap!<CR>
nnoremap <silent> <A-k> :wincmd k<CR>
nnoremap <silent> <A-j> :wincmd j<CR>
nnoremap <silent> <A-h> :wincmd h<CR>
nnoremap <silent> <A-l> :wincmd l<CR>
let g:FocusToggle = 0
nnoremap <silent> <Space> :if (g:FocusToggle == 0) \| :vertical res \| let g:FocusToggle=1 \| else \|winc =  \| let g:FocusToggle=0 \| endif<Bar>:echo<CR>

" NOTE: I don't know why this works, but adding a "^M"(ctrl-v ctrl-m in insert mode) makes this work as a toggle.
nnoremap <silent> <A-f> :simalt ~r<CR>:simalt ~x<CR>

" TODO: syntax highlighting for compile log
" TODO: add searching for build script
" TODO: make this asyncronous
if !exists("*Build")
function! Build()
    if match(expand("%"), "\.vimrc") > 0
        silent wall
        so %
        simalt ~x
    elseif (expand("%:p:h:t") ==? "colors") && (expand("%:e") ==? "vim")
        silent wall
        let schemename = expand("%:t:r")
        exe ":colo " . schemename
    else
        call setqflist([])
        silent wall
        echo "Compiling..."
        let Log = system('build')
        cgetexpr Log
        " wincmd h
        " TODO: set wrapping for quickfix buffer only
        bel cw
        echon "\r\rCompilation Finished!"
    endif
endfunction
endif

" Mapping Alt-m to run build.bat and Alt-n to go to next error
nnoremap <silent> <A-N> :cp<CR>
nnoremap <silent> <A-n> :cn<CR>
nnoremap <silent> <A-m> :call Build()<CR>
nnoremap <silent> <A-c> :call SourceToHeader(0)<CR>
nnoremap <silent> <A-C> :call SourceToHeader(1)<CR>

function! OpenScratchBuffer()
    let s:Scratchname = bufname("scratch")
    if (strlen(s:Scratchname)) > 0
        w
        exe ":buffer " . s:Scratchname
    else
        enew
        " exe \":enew" 
        file "scratch"
        setlocal buftype=nofile
        setlocal bufhidden=hide
        setlocal noswapfile
    endif
endfun

function! s:SwitchWindow()
    let OldWindow = winnr()
    wincmd l
    if winnr() == OldWindow
        wincmd h
    endif
endfun

function! OpenBufferOrFile(Filename, OtherWindow)
    let s:Bufname = bufname(a:Filename)
    if a:OtherWindow
        call s:SwitchWindow()
    endif
    if (strlen(s:Bufname)) > 0
        w
        exe ":buffer " . s:Bufname
    else
        exe ":e " . a:Filename
    endif
endfun

function! SourceToHeader(OtherWindow)
    if match(expand("%"), '\.cpp') > 0
        let l:flipname = substitute(expand("%"),'\.cpp\(.*\)','.h\1',"")
    elseif match(expand("%"), '\.h') > 0
        let l:flipname = substitute(expand("%"),'\.h\(.*\)','.cpp\1',"")
    endif

    if exists("l:flipname")
        call OpenBufferOrFile(l:flipname, a:OtherWindow)
    else
        echo "Unrecognized Source!"
    endif
endfun

" Autoindent options
set expandtab
set shiftwidth=4
set softtabstop=4
set smarttab
set autoindent
set cindent
set cinoptions=l1,g0,N-s,E-s,t0,(0,w1,Ws,m1,=0
" TODO: Maybe add color to statusline
set statusline=\ %f%m\%=\ %y\ %{&fileencoding?&fileencoding:&encoding}\[%{&fileformat}\]\ %p%%\ %l:%c\ 

filetype plugin on
set omnifunc=syntaxcomplete#Complete

function! HeaderSkeleton(filename)
    let l:HeaderMacro = toupper( substitute( substitute(a:filename, '\.h', '_H',""), '\.', '_', ""))
    call setline(1, '#if !defined(' . l:HeaderMacro . ')')
    call setline(2, '')
    call setline(3, '#define ' . l:HeaderMacro)
    call setline(4, '#endif')
endfun

function! SourceSkeleton(filename)
    call setline(1, '// ' . a:filename)
endfun

au BufNewFile *.h   call HeaderSkeleton(expand('%:t'))
au BufNewFile *.c   call SourceSkeleton(expand('%:t'))
au BufNewFile *.cpp call SourceSkeleton(expand('%:t'))

" Typing utilities
function! InsertFor(Signed, IndexName, IndexEnd, ...)
    " TODO: Maybe add '[u]int' vs '[u]int32' check
    if a:Signed
        let l:Type = 'i32'
    else
        let l:Type = 'u32'
    endif
    if a:0 > 0
        let l:IndexStart = a:1
    else
        let l:IndexStart = 0
    endif
    call append(line('.') - 1, [
                \ 'for('. l:Type . ' ' . a:IndexName . ' = ' . l:IndexStart . ';',
                \ '    ' . a:IndexName . ' < ' . a:IndexEnd . ';',
                \ '    ++' . a:IndexName . ')',
                \ '{'])
    normal k
    normal =3k
    normal j
endfun 
command! -nargs=+ Foru call InsertFor(0, <f-args>)
command! -nargs=+ For  call InsertFor(1, <f-args>)

nnoremap ,f :For 
nnoremap ,uf :Foru 
nnoremap <silent> <A-s> :call OpenScratchBuffer()<CR>

autocmd FileType c,cpp,java,scala let b:comment_leader = '//'
autocmd FileType sh,ruby,python   let b:comment_leader = '#'
autocmd FileType conf,fstab       let b:comment_leader = '#'
autocmd FileType tex              let b:comment_leader = '%'
autocmd FileType mail             let b:comment_leader = '>'
autocmd FileType vim              let b:comment_leader = '"'
function! CommentToggle()  " https://stackoverflow.com/a/22246318
    execute ':silent! s/\([ ]\)/' . escape(b:comment_leader,'\/') . '\1/'
    execute ':silent! s/^\( *\)' . escape(b:comment_leader,'\/') . ' \?' . escape(b:comment_leader,'\/') . '\?/\1/'
endfunction
map <F8> :call CommentToggle()<CR>




so $HOME/vimfiles/syntax/hlsl.vim
