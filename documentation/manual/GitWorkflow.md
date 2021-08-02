# Git Workflow

![](meta/git/workflow.png)

All the day-to-day work occurs on the **dev** branch, not worrying about much except pushing the features further or fixing bugs.  
When a state is considered stable and feature-complete according to a given goal, a temporary branch is created named according to the current version.  
Increments in the major, minor and patch numbers are chosen according to the nature of the changes that occured during the development, and wether source or binary compatibility was broken or not.  
For example, when **dev** is in a ready state at a given commit, a temporary branche named after the version **4.1.0** is created. Some cosmetic commits are allowed here, such as minor reformating or updating the version numbers in the documentation. The branch then follows the test procedure to ensure everything builds and that there is not errors at runtime, it can then be merger onto the **master** branch, where a release is created.  
Once the merge is confirmed and the repository is checked to be in a valid state, the temporary branch **4.1.0** can be deleted, so it does not persist forever in time.
