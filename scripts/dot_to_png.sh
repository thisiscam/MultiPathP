watchmedo shell-command \
    --patterns="*.dot" \
    --recursive \
    --command='dot -Tpng ${watch_src_path} -o ${watch_src_path}.png' \
	$1
