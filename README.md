# parserExpression
# I realized that algorithm not through the reverse Polish notation.
# I go through the line and open brackets. After i open max priority brackets i go back track into 
# last left brackets and go on.
# When i say open brackets i mean replacement of the expression between left and right brackets 
# (include brackets) with max priority to accounted expression between brackets and i return # the beginning of  # the string passed in function.