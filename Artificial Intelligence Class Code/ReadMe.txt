This is the folder that contains my small Artifical Intelligence programs!

The "big picture" of these projects are as follows:

---Shape Analogy Identification Project [Project 1 of 2]---
This project, written entirely in C++, asked us to create an AI
that was able to input text from a file that represented a series of lines,
or in some cases, circles.

The AI we were instructed to build was to take this input and determine 
what shapes were present within the diagram, as well as the multiple 
interpretations of the diagram.

Meaning, if there were intersecting lines within the diagram, the AI should be
able to understand that if it were given a diagram that looked like this:

    ---------------------------
   |                   /\      |
   |                  /  \     |
   |                 /    \    |
   |                /      \   |
   |               /        \  |
   |              /          \ |
    ---------------------------
                /______________\

There should be 2 distinct interpretations of this diagram. 

One with a simple rectange as well as a triange:

    ---------------------------
   |                           |
   |                           |
   |                           |
   |                           |
   |                           |
   |                           |
    ---------------------------



                       /\      
                      /  \     
                     /    \    
                    /      \   
                   /        \  
                  /          \ 
   				 /            \
                /______________\

And a seperate one that has an oddly shaped polygon, and a smaller triangle:

    ---------------------------
   |                           |
   |                           |
   |                           |
   |                           |
   |                           |
   |                           |
    -------------              -
                /______________\

                       /\      
                      /  \     
                     /    \    
                    /      \   
                   /        \  
                  /          \ 
                  ------------

The program should also be able to tell how the shapes of these diagrams relate to eachother:

Is one to the left of another?
Is one inside of another?
Is one above another?

As well as what type of shape they are. 

This information is contained within the "reports" folder for various test problems. The PDFs show 
the multiple interpretations, as well as the relations for each.


---Shape Analogy Project [Project 2 of 2]---

This project is a continuation of the "Shape Identification Project".

Written entirely in C++, again, this project utilized the code from the first and created 
an AI that could solve what is called "Geometric Analogy Problems."

For extensive background information about these types problems, 
this is a good resource:
https://onlinelibrary.wiley.com/doi/full/10.1111/j.1551-6709.2009.01052.x

This project was designed to read from a file that represented basic geometric anaolgy problems,
and choose what it believed to be the correct answer. 

The "a2-poznak.pdf" file contains documentation of the code written and the methods used to solve the 
problem.

---Unknown Authorship Project [Machine Learning]---

This project, written in C++, was designed to use machine learning to explore 
finding the likelihood of an author having written a text, given the information
known about the author's writing habits. 

Heavily leaning on statistical analysis of the words used, frequency at which they were used,
and usage in relation to other words. Background information on the methods used to determine authorship
are found here:
https://en.wikipedia.org/wiki/Naive_Bayes_classifier

---Decision Making Heuristic Project---

This project is fascinating!

The goal of this project was to create an AI that traversed a set of decisions based on the relative
utility of each decision. This can be expanded out for a variety of uses, but typically is 
viewed as a path-finding alogorithm imagined on a map. 

Background information is found here:

https://en.wikipedia.org/wiki/Markov_decision_process

------------------------------------

Hopefully, all of these were interesting!

Thanks.
Sam








