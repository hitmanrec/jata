const { app, BrowserWindow } = require('electron');
const path = require('path');
const { spawn } = require('child_process');
const fs = require('fs');

let mainWindow;
let serverProcess;

function createWindow() {
  mainWindow = new BrowserWindow({
    width: 1200,
    height: 800,
    webPreferences: {
      nodeIntegration: false,
      contextIsolation: true
    }
  });

  // In development, load from dev server
  // In production, load from built files
  const isDev = process.env.NODE_ENV === 'development';
  
  if (isDev) {
    mainWindow.loadURL('http://localhost:8080');
    mainWindow.webContents.openDevTools();
  } else {
    mainWindow.loadFile(path.join(__dirname, '../dist/index.html'));
  }

  mainWindow.on('closed', () => {
    mainWindow = null;
  });
}

function startServer() {
  // Determine server executable path
  const isDev = process.env.NODE_ENV === 'development';
  let serverPath;
  
  if (isDev) {
    serverPath = path.join(__dirname, '../server/jata_server.exe');
  } else {
    // In packaged app, server is in resources
    serverPath = path.join(process.resourcesPath, 'server/jata_server.exe');
  }

  // Check if server exists
  if (!fs.existsSync(serverPath)) {
    console.error('Server executable not found at:', serverPath);
    return;
  }

  // Start the C++ server
  serverProcess = spawn(serverPath, [], {
    cwd: path.dirname(serverPath),
    stdio: 'inherit'
  });

  serverProcess.on('error', (err) => {
    console.error('Failed to start server:', err);
  });

  serverProcess.on('exit', (code) => {
    console.log(`Server exited with code ${code}`);
  });
}

app.whenReady().then(() => {
  startServer();
  
  // Wait a bit for server to start, then create window
  setTimeout(() => {
    createWindow();
  }, 2000);

  app.on('activate', () => {
    if (BrowserWindow.getAllWindows().length === 0) {
      createWindow();
    }
  });
});

app.on('window-all-closed', () => {
  // Kill server process
  if (serverProcess) {
    serverProcess.kill();
  }
  
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

app.on('before-quit', () => {
  // Ensure server is killed on quit
  if (serverProcess) {
    serverProcess.kill();
  }
});