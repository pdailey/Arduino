" Vim package manager {{{1
" Replaces neobundle package manager
"  ----------------------------------------------------------------------------
" Required:
set runtimepath^=/Users/peterdailey/.vim/bundle/repos/github.com/Shougo/dein.vim

" Required:
call dein#begin(expand('/Users/peterdailey/.vim/bundle'))

" Let dein manage dein
" Required:
call dein#add('Shougo/dein.vim')

" Add or remove your plugins here:
" Text snippets
" TODO: Get this to works one day..
"call dein#add('valloric/youcompleteme')

call dein#add('matze/vim-move')

" Change the surrounding text object
call dein#add('tpope/vim-surround')

" Allows some plugin commands to be repeated with .
call dein#add('tpope/vim-repeat')

" This plug-in provides automatic closing of quotes, parenthesis, brackets,
" etc.
call dein#add('Raimondi/delimitMate')

" Working with variants of a word
call dein#add('tpope/vim-abolish')

" Expands visual selection.
call dein#add('terryma/vim-expand-region')
call dein#add('godlygeek/tabular')

"Commonly misspelled words and their corrections
call dein#add('chip/vim-fat-finger')

" Open markdown in browser
call dein#add('shime/vim-livedown')

" Colorscheme
call dein#add('vim-scripts/Solarized')

" Required:
call dein#end()

" Required:
syntax on
filetype plugin indent on

" If you want to install not installed plugins on startup.
if dein#check_install()
  call dein#install()
endif

"End dein Scripts-------------------------


"  ----------------------------------------------------------------------------
" To be sorted {{{1
"  ----------------------------------------------------------------------------
set title " Set title of window to file name
set cursorline          " highlight current line

set showmatch           " highlight matching [{()}]


" Solarized settings
syntax enable
let g:solarized_termtrans = 1
set background=dark
colorscheme solarized

" Resize splits when window is resized
au VimResized * exe "normal \<c-w>="

" Vim jumps to the last position when  reopening a file
if has("autocmd")
  au BufReadPost * if line("'\"") > 1 && line("'\"") <= line("$") | exe "normal! g'\"" | endif
endif

" Show context when scrolling vertically.
set scrolloff=5

" ================ Persistent Undo ==================
" Keep undo history across sessions, by storing in file.
if has('persistent_undo')
silent !mkdir ~/.vim/backups > /dev/null 2>&1
set undodir=~/.vim/backups
set undofile
endif

" Spelling
set spell               " Spelling always on
set spelllang=en        " Spelling language is English

" Shortcuts using <leader>
" TODO: Is this useful?
map <leader>ss ]s
map <leader>sb [s
map <leader>sa zg
map <leader>s? z=

" Tabs
set expandtab           " tabs are spaces
set tabstop=2           " number of visual spaces per TAB
set softtabstop=4       " number of spaces in tab when editing


" Search
set ignorecase          " Ignore case when searching
set smartcase           " ...Unless we use a capital
set hlsearch            " Highlight search results
set incsearch           " Search acts like search in modern browsers

" Filetype Specific
autocmd Filetype c   setlocal ts=4 sts=4 sw=4
autocmd Filetype cpp setlocal ts=2 sts=2 sw=2

" Whitespace
autocmd BufWritePre * :%s/\s\+$//e

" Changes how line numbers are displayed depending upon mode.
set relativenumber
autocmd InsertEnter * :set number
autocmd InsertLeave * :set relativenumber

au FocusLost * :wa      " Save when focus is lost
set noswapfile

" ============== Mappings ===============
" NOTE : Comments cannot be made in the same line as mappings!

"Map leader to spacebar
let mapleader="\<Space>"

" move vertically by visual line
nnoremap j gj
nnoremap k gk

" Go to beginning and end of line
noremap H ^
noremap L g_

" Matching is done with tabs. Doesn't conflict with usage in insert mode
map <tab> %

" Quickly edit/reload the vimrc file
nmap <silent> <leader>ev :e $MYVIMRC<CR>
nmap <silent> <leader>sv :so $MYVIMRC<CR>


" Mark the 80th column in editor
if (exists('+colorcolumn'))
   set colorcolumn=80
   highlight ColorColumn ctermbg=8
endif

" Backspace works in insert
set backspace=indent,eol,start

" ========== Learning Zone ====================
" Copy and paste to system keyboard with <Leader>y and <Leader>p
vmap <Leader>y "+y
vmap <Leader>d "+d
nmap <Leader>p "+p
nmap <Leader>P "+P
vmap <Leader>p "+p
vmap <Leader>P "+P

" Use region expanding
vmap v <Plug>(expand_region_expand)
vmap <C-v> <Plug>(expand_region_shrink)

" Enter Visual mode with <Leader><Leader>
" nmap <Leader><Leader> V


" Spelling
" find next misspelled word
nnoremap <leader>s [sz=
" autocomplete words by pressing <C-n> or <C-p>
set complete+=kspell

" Replacing Variables
" Substitute all occurrences of word under cursor
" type the replacement word, then hit enter
" Will confirm each word before replacing
nnoremap <Leader>r :%s/\<<C-r><C-w>\>//gc<Left><Left><Left>


" Window splitting
" TODO: learn this!
" Open new split panes to right and bottom
set splitbelow
set splitright

" Navigate splits with arrow keys
nnoremap <Right> <C-w>l
nnoremap <Left> <C-w>h
nnoremap <Up> <C-w>k
nnoremap <Down> <C-w>j


" ============== Plugins ==============

" Tabularize
" TODO: Tabularize has a lot more capabilities
" will call the :Tabularize command each time you insert a | character.
inoremap <silent> <Bar>   <Bar><Esc>:call <SID>align()<CR>a

" Align Tables. Calls Tabularize on use of pipe character.
function! s:align()
  let p = '^\s*|\s.*\s|\s*$'
  if exists(':Tabularize') && getline('.') =~# '^\s*|' && (getline(line('.')-1) =~# p || getline(line('.')+1) =~# p)
    let column = strlen(substitute(getline('.')[0:col('.')],'[^|]','','g'))
    let position = strlen(matchstr(getline('.')[0:col('.')],'.*|\s*\zs.*'))
    Tabularize/|/l1
    normal! 0
    call search(repeat('[^|]*|',column).'\s\{-\}'.repeat('.',position),'ce',line('.'))
  endif
endfunction

" ============== Arduino ==============
autocmd BufNewFile,BufReadPost *.ino,*.pde set filetype=cpp

" ============== Markdown ==============
" Force vim to read .mkd as markdown.
autocmd BufNewFile,BufReadPost *.md set filetype=markdown

" ============== Par ==============
" Used to reformat text
" run with command 'gq'
" set line width to 80
set formatprg=par\ -w80

" Livedown
" should markdown preview get shown automatically upon opening markdown buffer
let g:livedown_autorun = 0

" should the browser window pop-up upon previewing
let g:livedown_open = 1

" the port on which Livedown server will run
let g:livedown_port = 1337

nmap <leader>md :LivedownToggle<CR>

