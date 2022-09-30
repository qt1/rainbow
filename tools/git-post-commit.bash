#!/bin/sh
#

#executed by .git/hooks/post-commit
#bash git_hooks/git-post-commit.bash

rev=$(git describe --abbrev=10 --dirty --always --tags)
rev_date=$(date +'%Y-%m-%d %H:%M')
dest='main/revision.h'

echo '// revision info' > $dest
echo '#pragma once' >> $dest
echo '#define REVISION "'$rev'"'>> $dest
echo '#define REVISION_DATE "'$rev_date'"'>> $dest


