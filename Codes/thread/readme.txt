Four variants of a threaded linked list, all of which exhibit a race
condition, although some implementations exhibit the conflict much less
frequently than others (list3).

threaded_list1 -- search for tail each time
threaded_list2 -- remember last found end and insert from there
threaded_list3 -- keep global tail pointer
threaded_list4 -- like 2, but follow to end of list from that point

