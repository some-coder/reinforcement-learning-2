# -- Input -------------------------------------------------------------------------------------------------------------

#' Returns the column names for the given data file.
#' 
#' @param file.name The file
input.frame.column.names <- function(file.name) {
  switch (file.name,
          'average-policies' = c("run.id", "maze.id", "algorithm", "x", "y", "action",
                                 "selection.probability"),
          'exploitation' = c("maze.id", "algorithm", "total.reward"),
          'timings' = c("run.id", "maze.id", "algorithm", "iteration", "millisecond.timing"),
          'progression' = c("run.id", "time", "maze.id", "algorithm", "episode.reward"));
}

#' Returns a frame of the given data created by the project's main program.
#' 
#' Column names automatically get supplied.
#' 
#' @param file.name The name of the data file in subdirectory 'data'.
#' @return The frame.
input.frame.from.program <- function(file.name) {
  cat(paste("  Reading '", file.name, "'.\n", sep = ""));
  with.extension <- paste('data/', file.name, '.csv', sep = '')
  f <- read.csv(with.extension, stringsAsFactors = FALSE, header = FALSE);
  colnames(f) <- input.frame.column.names(file.name);
  return(f);
}

# -- Conversion --------------------------------------------------------------------------------------------------------

algorithm.abbreviation <- function(algorithm.name) {
  switch (algorithm.name,
          "Random Policy"                 = "RND",
          "Asynchronous Policy Iteration" = "API",
          "Asynchronous Value Iteration"  = "AVI",
          "Monte Carlo Every Visit"       = "MEV",
          "Monte Carlo First Visit"       = "MFV",
          "Synchronous Policy Iteration"  = "SPI",
          "Synchronous Value Iteration"   = "SVI",
          "Monte Carlo Exploring Starts"  = "MES",
          "Time-difference SARSA"         = "TDS",
          "Time-difference Q-learning"    = "TDQ");
}

algorithm.abbreviations <- function(algorithm.names) {
  cat("  Abbreviating algorithm names.\n");
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
  cat("  Aggregating epoch timings into run times.\n");
  return(aggregate(millisecond.timing ~ run.id + maze.id + algorithm, data = timings, FUN = sum));
}

#' Creates a boxplot of total time spent by algorithms on this maze.
#' 
#' @param total.timings Total times of the algorithms.
#' @param maze.id The maze ID of which to make boxplots for.
maze.timing.boxplot <- function(total.timings, maze.id) {
  cat(paste("    ", "Building boxplot of maze '", maze.id, "'.\n", sep = ""));
  m <- maze.id;
  sub <- subset(total.timings, maze.id == m);
  boxplot(millisecond.timing ~ algorithm,
          data   = sub,
          main  = paste("Run time totals per algorithm\nfor ", maze.id, sep = ''),
          xlab = "",
          ylab = "Total run time (milliseconds)",
          las = 2,
          cex.sub = 2,
          cex.lab = 1.5,
          cex.main = 1.5);
}

#' Plots run time totals per algorithm, per maze.
#' 
#' @param total.timings Timing totals per algorithm, per run.
maze.timing.boxplots <- function(total.timings) {
  maze.amount <- length(unique(total.timings$maze.id));
  prepare.window.for.progression.plots(maze.amount);
  cat("  Building run times boxplots.\n");
  for (maze in unique(total.timings$maze.id)) {
    maze.timing.boxplot(total.timings, maze);
  }
  par(mfrow = c(1, 1));
}

maze.reward.boxplot <- function(gains, maze.id) {
  cat(paste("    ", "Building boxplot of maze '", maze.id, "'.\n", sep = ""));
  m <- maze.id;
  sub <- subset(gains, maze.id == m);
  boxplot(total.reward ~ algorithm,
          data = sub,
          main = paste("Gains per algorithm\nfor ", maze.id, sep = ''),
          xlab = "",
          ylab = "Total gains",
          las = 2,
          cex.sub = 2,
          cex.lab = 1.5,
          cex.axis = 1.5,
          cex.main = 1.5);
}

maze.reward.boxplots <- function(gains) {
  maze.amount <- length(unique(gains$maze.id));
  prepare.window.for.progression.plots(maze.amount);
  cat("  Building algorithm average policy rewards boxplots.\n");
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
  return(test$p.value < 1e-2);
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

make.empty.plot <- function(data, maze) {
  x.limits <- range(data$time);
  y.limits <- range(data$episode.reward);
  plot(c(0, 0),
       type = "n",
       xlab = "Time (Epochs)",
       ylab = "Average total reward",
       main = paste("Time vs. Average Total Reward\nOn ", maze, sep = ""),
       xlim = x.limits,
       ylim = y.limits,
       panel.first = grid(),
       cex.sub = 2,
       cex.lab = 2,
       cex.main = 1.5);
}

prepare.window.for.progression.plots <- function(maze.amount) {
  dims <- c(1, 1);
  if (maze.amount == 1) {
    dims <- c(1, 1);
  } else if (maze.amount == 2) {
    dims <- c(1, 2);
  }  else if (maze.amount == 3 || maze.amount == 4) {
    dims <- c(2, 2);
  } else if (maze.amount == 5 || maze.amount == 6) {
    dims <- c(3, 2);
  }  else if (maze.amount == 7 || maze.amount == 8) {
    dims <- c(4, 2);
  }
  par(mfrow = dims);
}

algorithm.color <- function(algorithm) {
  switch(algorithm,
         "MCES" = "#396ab1",
         "MCFV" = "#da7c30",
         "MCEV" = "#3e9651",
         "TDS"  = "#cc2529",
         "TDQ"  = "#535154");
}

algorithm.colors <- function(algorithms) {
  colors <- c();
  for (alg in algorithms) {
    colors <- c(colors, algorithm.color(alg));
  }
  return(colors);
}

maze.progression.plot <- function(data, maze) {
  make.empty.plot(data, maze);
  for (alg in unique(data$algorithm)) {
    single <- subset(data, algorithm == alg, select = c(time, episode.reward));
    lines(single$episode.reward ~ single$time, col = algorithm.color(alg));
  }
  x.limits <- range(data$time);
  y.limits <- range(data$episode.reward);
  x.pos <- x.limits[1] + (7/10) * abs(x.limits[2] - x.limits[1]);
  y.pos <- y.limits[1] + (5/10) * abs(y.limits[2] - y.limits[1]);
}

maze.progression.plots <- function(progression) {
  maze.amount <- length(unique(progression$maze.id));
  prepare.window.for.progression.plots(maze.amount);
  data <- aggregate(episode.reward ~ time + maze.id + algorithm, data = progression, FUN = mean);
  for (maze in unique(progression$maze.id)) {
    sub <- subset(data, maze.id == maze);
    maze.progression.plot(sub, maze);
  }
  par(mfrow = c(1, 1));
}

# -- Output ------------------------------------------------------------------------------------------------------------

#' Runs this script's main program.
#' 
#' @return The exit signal. Either 'success' or 'failure'.
main <- function() {
  if (!file.exists('figures/')) {
    dir.create('figures/');
  }
  timings <- input.frame.from.program('timings'); 
  rewards <- input.frame.from.program('exploitation');
  progression <- input.frame.from.program('progression');
  totals  <- total.timings(timings);
  postscript("figures/time-boxplot.eps", horizontal = FALSE,
             onefile = FALSE, paper = 'special', height = 10,
             width = 12);
  maze.timing.boxplots(totals);
  dev.off();
  postscript("figures/exploitation-boxplot.eps", horizontal = FALSE,
             onefile = FALSE, paper = 'special', height = 10,
             width = 10);
  maze.reward.boxplots(rewards);
  dev.off();
  prepare.window.for.progression.plots(length(unique(progression$maze.id)));
  postscript("figures/progression-plot.eps", horizontal = FALSE,
             onefile = FALSE, paper = 'special', height = 10,
             width = 10);
  maze.progression.plots(progression);
  dev.off();
  return("success");
}

main()
