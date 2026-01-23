#pragma once

// Board dimension
static constexpr int N = 3;

// Number of features used in the linear value-function approximation
static constexpr int NUM_FEATS = 10;

// Initial value assigned to all weights before training begins
static constexpr double INIT_W = 0.1;

// Learning rate for the LMS weight-update rule
static constexpr double ALPHA  = 0.01;
