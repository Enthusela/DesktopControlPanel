# Multi-stage build for Node.js RGB controller web app
FROM node:18-slim AS base

# Install dependencies needed for serialport native modules
RUN apt-get update && apt-get install -y \
    python3 \
    make \
    g++ \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy package files
COPY package*.json ./

# Install production dependencies
RUN npm ci --only=production

# Development stage
FROM base AS development

# Install all dependencies including devDependencies
RUN npm ci

# Copy application files
COPY . .

# Expose the web server port
EXPOSE 8080

# Use nodemon for hot reload in development
RUN npm install -g nodemon
CMD ["nodemon", "--watch", ".", "--ext", "js,html", "webserver.js"]

# Production stage
FROM base AS production

# Copy application files
COPY . .

# Expose the web server port
EXPOSE 8080

# Run the application
CMD ["node", "webserver.js"]
