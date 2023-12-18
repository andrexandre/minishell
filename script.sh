#!/bin/bash

##startup
#gnome-terminal
#google-chrome
#/nfs/homes/analexan/Downloads/...

##install minishell testers

##fix settings

##check if:
#ZSH_THEME="simple"
#export PATH="$PATH:/home/andrealex/.local/bin"

#ln -s ~/.vscode vscode
: <<'COMMENT'
cat << EOF >> ~/.zshrc
#My custom aliases
alias 42exam='bash -c "$(curl https://grademe.fr)"'
gacp() {
	if [ -z "$1" ]; then
		git add . && git commit -m "Added all" && git push
	else
		git add . && git commit -m $1 && git push
	fi
}
alias fa='find . -type f ! -name "*.?*" -not -path "*Makefile*" -not -path "./.git/*"'
alias fgit='find . -type d -name ".git"'
alias ccc='cc -Wall -Wextra -Werror *.c && ./a.out'
alias c='code .'
alias q='exit'
gac() {
	git clone "git@github.com:andrexandre/$1.git" "$1"
}
EOF
COMMENT
