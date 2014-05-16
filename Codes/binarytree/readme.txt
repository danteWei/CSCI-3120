binarytree1 -- basic binary tree code
binarytree2 -- binary tree with separate search function (note that it has a bug ... search just returns current while functions like "add" need the parent instead)
binarytree3 -- the code of binarytree2 but with the parent information fixed
binarytree4 -- binarytree3 code with the search work done inside a function that assumes a global lock has been acquired (like a private method).  Now we can put a global lock in add, search, and delete without calling wait() twice.

