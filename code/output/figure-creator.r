# -- Input -------------------------------------------------------------------------------------------------------------

#' Returns the column names for the given data file.
#' 
#' @param file.name The file
input.frame.column.names <- function(file.name) {
    switch (file.name,
            'policies' = c("run.id", "maze.id", "algorithm", "x", "y", "action",
                                "selection.probability"),
            'policy-rewards' = c("maze.id", "algorithm", "total.reward"),
            'timings' = c("run.id", "maze.id", "algorithm", "iteration", "millisecond.timing"));
}

#' Returns a frame of the given data created by the project's main program.
#' 
#' Column names automatically get supplied.
#' 
#' @param file.name The name of the data file in subdirectory 'data'.
#' @return The frame.
input.frame.from.program <- function(file.name) {
    with.extension <- paste('data/', file.name, '.csv', sep = '')
    f <- read.csv(with.extension, stringsAsFactors = FALSE, header = FALSE);
    colnames(f) <- input.frame.column.names(file.name);
    return(f);
}

# -- Conversion --------------------------------------------------------------------------------------------------------

algorithm.abbreviation <- function(algorithm.name) {
    switch (algorithm.name,
            "Asynchronous Policy Iteration" = "API",
            "Asynchronous Value Iteration"  = "AVI",
            "Monte Carlo Every Visit"       = "MEV",
            "Monte Carlo First Visit"       = "MFV",
            "Synchronous Policy Iteration"  = "SPI",
            "Synchronous Value Iteration"   = "SVI",
            "Monte Carlo Exploring Starts"  = "MES");
}

algorithm.abbreviations <- function(algorithm.names) {
    abbreviations <- c();
    for (name in algorithm.names) {
        abbreviations <- c(abbreviations, algorithm.abbreviation(name));
    }
    return(abbreviations);
}

#' Returns a frame of the timing data in which individual epochs are collapsed into totals.
#' 
#' @param timings The timing data per epoch.
#' @return The frame, collapsed.
total.timings <- function(timings) {
    return(aggregate(millisecond.timing ~ run.id + maze.id + algorithm, data = timings, FUN = sum));
}

#' Creates a boxplot of total time spent by algorithms on this maze.
#' 
#' @param total.timings Total times of the algorithms.
#' @param maze.id The maze ID of which to make boxplots for.
maze.timing.boxplot <- function(total.timings, maze.id) {
    m <- maze.id;
    sub <- subset(total.timings, maze.id == m);
    boxplot(millisecond.timing ~ algorithm,
            data   = sub,
            main  = paste("Run time totals per algorithm\nfor ", maze.id, sep = ''),
            xlab = "Algorithm",
            ylab = "Total run time (milliseconds)",
            las = 2);
}

#' Plots run time totals per algorithm, per maze.
#' 
#' @param total.timings Timing totals per algorithm, per run.
maze.timing.boxplots <- function(total.timings) {
    maze.amount <- length(unique(total.timings$maze.id));
    par(mfrow = c(maze.amount, 1));
    for (maze in unique(total.timings$maze.id)) {
        maze.timing.boxplot(total.timings, maze);
    }
    par(mfrow = c(1, 1));
}

maze.reward.boxplot <- function(gains, maze.id) {
    m <- maze.id;
    sub <- subset(gains, maze.id == m);
    boxplot(total.reward ~ algorithm,
            data = sub,
            main = paste("Gains per algorithm\nfor ", maze.id, sep = ''),
            xlab = "Algorithm",
            ylab = "Total gains",
            las = 2);
}

maze.reward.boxplots <- function(gains) {
    maze.amount <- length(unique(gains$maze.id));
    par(mfrow = c(maze.amount, 1));
    for (maze in unique(gains$maze.id)) {
        maze.reward.boxplot(gains, maze);
    }
    par(mfrow = c(1, 1));
}

timing.test <- function(timings, maze, a1, a2) {
    a1.sub <- subset(timings, maze.id == maze & algorithm == a1);
    a2.sub <- subset(timings, maze.id == maze & algorithm == a2);
    timing <- c(a1.sub$millisecond.timing, a2.sub$millisecond.timing);
    type   <- c(a1.sub$algorithm, a2.sub$algorithm);
    d      <- data.frame("millisecond.timing" = timing, "type" = type);
    test   <- wilcox.test(millisecond.timing ~ type, data = d);
    return(test$p.value < 0.01);
}

timing.tests <- function(timings, maze.id) {
    for (a1 in unique(timings$algorithm)) {
        for (a2 in unique(timings$algorithm)) {
            if (a1 != a2) {
                cat(paste('Algorithm A: ', a1, ', Algorithm B: ', a2, '.\n', sep = ''));
                cat(paste(timing.test(timings, maze.id, a1, a2), '.\n', sep = ''));
            }
        }
    }
}

reward.test <- function(rewards, maze, a1, a2) {
    a1.sub <- subset(rewards, maze.id == maze & algorithm == a1);
    a2.sub <- subset(rewards, maze.id == maze & algorithm == a2);
    reward <- c(a1.sub$total.reward, a2.sub$total.reward);
    type   <- c(a1.sub$algorithm, a2.sub$algorithm);
    d      <- data.frame("total.reward" = reward, "type" = type);
    test   <- wilcox.test(total.reward ~ type, data = d);
    return(test$p.value < 0.01);
}

rewards.tests <- function(rewards, maze.id) {
    for (a1 in unique(rewards$algorithm)) {
        for (a2 in unique(rewards$algorithm)) {
            if (a1 != a2) {
                cat(paste('Algorithm A: ', a1, ', Algorithm B: ', a2, '.\n', sep = ''));
                cat(paste(reward.test(rewards, maze.id, a1, a2), '.\n', sep = ''));
            }
        }
    }
}

# -- Output ------------------------------------------------------------------------------------------------------------

#' Runs this script's main program.
#' 
#' @return The exit signal. Either 'success' or 'failure'.
main <- function() {
    timings <- input.frame.from.program('timings'); 
    rewards <- input.frame.from.program('policy-rewards');
    totals  <- total.timings(timings);
    totals$algorithm <- algorithm.abbreviations(totals$algorithm);
    postscript("figures/time-boxplot.eps", horizontal = FALSE,
               onefile = FALSE, paper = 'special', height = 10,
               width = 10);
    maze.timing.boxplots(totals);
    dev.off();
    postscript("figures/gains.eps", horizontal = FALSE,
               onefile = FALSE, paper = 'special', height = 10,
               width = 10);
    rewards$algorithm <- algorithm.abbreviations(rewards$algorithm);
    maze.reward.boxplots(rewards);
    dev.off();
    return("success");
}

main()