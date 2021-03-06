% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/sim_farming.R
\name{run_farm_sim}
\alias{run_farm_sim}
\title{Initialise individuals and simulate farming}
\usage{
run_farm_sim(
  mine_output,
  N = 1000,
  xdim = 100,
  ydim = 100,
  repro = "sexual",
  neutral_loci = 0,
  max_age = 9,
  min_age_move = 0,
  max_age_move = 9,
  min_age_reproduce = 0,
  max_age_reproduce = 9,
  min_age_feed = 0,
  max_age_feed = 9,
  food_consume = 0.25,
  pesticide_consume = 0.1,
  rand_age = FALSE,
  move_distance = 1,
  food_needed_surv = 0.25,
  pesticide_tolerated_surv = 0.1,
  food_needed_repr = 0,
  pesticide_tolerated_repr = 0,
  reproduction_type = "lambda",
  mating_distance = 1,
  lambda_value = 1,
  movement_bouts = 1,
  selfing = TRUE,
  feed_while_moving = FALSE,
  mortality_type = 0,
  age_food_threshold = NA,
  age_pesticide_threshold = NA,
  farms = 4,
  time_steps = 100,
  mutation_pr = 0,
  crossover_pr = 0,
  mutation_type = 0,
  net_mu_layers = 0,
  net_mu_dir = 0,
  mutation_direction = 0,
  crop_rotation_type = 2,
  crop_rotation_time = 1,
  pesticide_rotation_type = 2,
  pesticide_rotation_time = 1,
  crop_per_cell = 1,
  pesticide_per_cell = 1,
  crop_sd = 0,
  pesticide_sd = 0,
  crop_min = 0,
  crop_max = 1000,
  pesticide_min = 0,
  pesticide_max = 1000,
  crop_number = 2,
  pesticide_number = 1,
  print_inds = FALSE,
  print_gens = TRUE,
  print_last = TRUE,
  K_on_birth = 0
)
}
\arguments{
\item{mine_output}{The output from mine_gmatrix}

\item{N}{Number of individuals to be initialised}

\item{xdim}{Horizontal dimensions of the landscape}

\item{ydim}{Vertical dimensions of the landscape}

\item{repro}{Type of reproduction allowed: "asexual", "sexual", and
"biparental". Note that if repro > 0, this causes a diploid genome.}

\item{neutral_loci}{The number of neutral loci individuals have}

\item{max_age}{The maximum age of an individual}

\item{min_age_move}{The minimum age at which an individual can move}

\item{max_age_move}{The maximum age at which an individual can move}

\item{min_age_reproduce}{The minimum age which an individual can reproduce}

\item{max_age_reproduce}{The maximum age which an individual can reproduce}

\item{min_age_feed}{The minimum age at which an individual feeds}

\item{max_age_feed}{The maximum age at which an individual feeds}

\item{food_consume}{The amount of food consumed during feeding}

\item{rand_age}{Initialise individuals with a random age}

\item{move_distance}{Maximum cells moved in one bout of movement}

\item{food_needed_surv}{Food needed to survive (if over min_age_feed)}

\item{pesticide_tolerated_surv}{Pesticide tolerated by individual}

\item{food_needed_repr}{Food needed to reproduce 1 offspring}

\item{pesticide_tolerated_repr}{Pesticide tolerated to allow reproduction}

\item{reproduction_type}{Poisson reproduction ("lambda") vs "food_based"}

\item{mating_distance}{Distance in cells within which mate is available}

\item{lambda_value}{individual value for poisson reproduction}

\item{movement_bouts}{Number of bouts of movement per time step}

\item{selfing}{If sexual reproduction, is selfing allowed?}

\item{feed_while_moving}{Do individuals feed after each movement bout?}

\item{mortality_type}{Type of mortality (currently only one option)}

\item{age_food_threshold}{Age at which food threshold is enacted}

\item{age_pesticide_threshold}{Age at which pesticide threshold is enacted}

\item{farms}{How many farms should there be on the landscape?}

\item{time_steps}{Time steps in the simulation}

\item{mutation_pr}{Probability of a loci mutating}

\item{crossover_pr}{Probability of crossover at homologous loci}

\item{mutation_type}{Type of mutation used}

\item{net_mu_layers}{Layers of the network allowed to mutate}

\item{net_mu_dir}{Layers mutate from loci to (1) or traits back (0)}

\item{mutation_direction}{Is mutation directional (unlikely to need)}

\item{crop_rotation_type}{None (0) or random (1) rotation of crop type}

\item{crop_rotation_time}{How frequently are the crops rotated?}

\item{crop_per_cell}{How much crop is put on a single cell?}

\item{pesticide_per_cell}{How much pesticide is put on a single cell?}

\item{crop_sd}{What is the standard deviation of crop on a cell?}

\item{pesticide_sd}{What is the standard deviation of pesticide on a cell?}

\item{crop_min}{What is the minimum crop amount allowed per cell?}

\item{crop_max}{What is the maximum crop amount allowed per cell?}

\item{pesticide_min}{What is the minimum pesticide amount allowed per cell?}

\item{pesticide_max}{What is the maximum pesticide amount allowed per cell?}

\item{crop_number}{How many crops exist on the landscape?}

\item{pesticide_number}{How many pesticides are applied on the landscape?}

\item{print_inds}{Should the full list of individuals be printed? (CAREFUL)}

\item{print_gens}{Should a summary of each time step be printed?}

\item{print_last}{Should the last time step of individuals be printed?}

\item{K_on_birth}{Is there a carrying capacity applied on newborns?}
}
\value{
prints a simulation file and output
}
\description{
Initialises a new set of individuals and then simulates farming over time
}
