#!/bin/bash
for branch in $(git branch -r | grep 'origin/ai/' | sed 's/origin\///'); do
    git push origin --delete "$branch"
done
git branch | grep 'ai/' | xargs git branch -D
