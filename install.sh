NAV_LINE='[ -f "$HOME/.navrc" ] && source "$HOME/.navrc"'

if ! grep -Fxq "$NAV_LINE" "$HOME/.bashrc"; then
    echo "" >> "$HOME/.bashrc"
    echo "# nav tool" >> "$HOME/.bashrc"
    echo "$NAV_LINE" >> "$HOME/.bashrc"
    echo "Added nav to .bashrc"
fi

