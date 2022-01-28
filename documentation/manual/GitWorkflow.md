# Git Workflow

![](meta/git/workflow.png)

All the day-to-day work occurs on the **dev** branch, not worrying about much except pushing the features further or fixing bugs.  
When a state is considered stable and feature-complete according to a given goal, a temporary branch is created named according to the current version.  
Increments in the major, minor and patch numbers are chosen according to the nature of the changes that occured during the development, and wether source or binary compatibility was broken or not.  
For example, when **dev** is in a ready state at a given commit, a temporary branche named after the version **4.1.0** is created. Some cosmetic commits are allowed here, such as minor reformating or updating the version numbers in the documentation. The branch then follows the test procedure to ensure everything builds and that there is not errors at runtime, it can then be merger onto the **master** branch, where a release is created.  
Once the merge is confirmed and the repository is checked to be in a valid state, the temporary branch **4.1.0** can be deleted, so it does not persist forever in time.

## Release process:
Details of the release process.

When you're ready to make a release, here is the step by step process:
    - You finished a stable, feature-complete state of the library according to a set goal on the branch dev.
    - You ensure it builds and runs
    - You make a tag on the latest commit on dev called dev-stable-Major.Minor.Patch
    - You make a branch called Major.Minor.Patch
    - You adjust cosmetic changes such as readme or version number if not done previously.
    - You ensure it builds and runs
    - Your merge-squash the branch called Major.Minor.Patch onto the branch release
    - In case of conflicts you checkout "theirs"
    - You ensure it builds and runs
    - You stage and commit the files with the commit name "release-Major.Minor.Patch"
    - You ensure it builds and runs
    - You make a tag on this commit on release called "release-Major.Minor.Patch"
    - You push all tags and branches involved in this process

```d
cd ULIS
git checkout dev
git fetch & git rebase
git tag "dev-stable-5.0.9"
git push --tags
git checkout -b 5.0.9
git push --set-upstream origin 5.0.9
git checkout release
git fetch & git rebase
git rm -r .
git checkout 5da1c06d318d19a31a2f26f9a09696c8de130184 -- . # the hash being the one from the latest commit of the newly created 5.0.9 branch
git commit -m "release-5.0.9"
git tag "release-5.0.9"
git push
git push --tags
exit
```

