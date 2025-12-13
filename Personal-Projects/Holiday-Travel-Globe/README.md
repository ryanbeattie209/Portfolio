# 🌍 Travel Memory Globe

An interactive 3D globe application built with React and Three.js that lets you organize and view your travel photos by country. Click on any country marker to create a beautiful photo album of your memories from that destination.

![Travel Memory Globe](https://img.shields.io/badge/React-18.2.0-blue)
![TypeScript](https://img.shields.io/badge/TypeScript-5.0-blue)
![Three.js](https://img.shields.io/badge/Three.js-r128-green)
![License](https://img.shields.io/badge/License-MIT-yellow)

## ✨ Features

- 🌐 **Interactive 3D Globe** - Drag to rotate and explore
- 📍 **20+ Country Markers** - Pre-loaded with popular travel destinations
- 📸 **Photo Albums** - Upload and organize multiple photos per country
- 🖼️ **Photo Gallery** - Navigate through your memories with arrow keys and thumbnails
- 📊 **Travel Statistics** - Track countries visited and total photos
- 🎨 **Beautiful UI** - Space-themed design with smooth animations
- 💾 **Session Storage** - Photos persist during your browsing session
- 🗑️ **Easy Management** - Delete photos with one click
- 📱 **Responsive Design** - Works on desktop and tablet devices

## 🚀 Quick Start

### Prerequisites

- Node.js (v14 or higher)
- npm or yarn package manager

### Installation

1. **Clone or download the project:**
   ```bash
   git clone <your-repo-url>
   cd travel-memory-globe
   ```

2. **Install dependencies:**
   ```bash
   npm install
   ```

3. **Start the development server:**
   ```bash
   npm run dev
   ```

4. **Open your browser:**
   ```
   http://localhost:3000
   ```

## Image of Globe
<img width="513" height="461" alt="image" src="https://github.com/user-attachments/assets/cdeee2b7-8b28-42f8-b2a6-f3347eeb67fb" />


## 📖 Usage

### Adding Photos

1. **Rotate the globe** by clicking and dragging
2. **Click on any red country marker** to open the photo album
3. **Click "Upload Photos"** button
4. **Select one or multiple images** from your device
5. **Navigate through photos** using arrow buttons or thumbnails

### Managing Photos

- **View photos**: Click thumbnails to jump to specific photos
- **Delete photos**: Click the red X button on any photo
- **Close album**: Click the X in the top-right corner

### Supported Countries

The globe includes markers for 20 popular destinations:
- United States
- United Kingdom
- France
- Germany
- Italy
- Spain
- Japan
- China
- Australia
- Brazil
- Canada
- India
- Russia
- Mexico
- South Africa
- Egypt
- Thailand
- Greece
- Turkey
- Argentina

## 🏗️ Project Structure

```
travel-memory-globe/
├── public/
│   └── index.html              # HTML entry point with Three.js CDN
├── src/
│   ├── App.tsx                 # Main component with globe & photo logic
│   ├── index.tsx               # React application entry point
│   └── styles.css              # Complete application styling
├── .gitignore                  # Git ignore configuration
├── package.json                # Project dependencies and scripts
├── tsconfig.json               # TypeScript configuration
├── tsconfig.node.json          # TypeScript config for Vite tooling
└── README.md                   # Project documentation (this file)
```

## 🛠️ Built With

### Core Technologies
- **[React 18](https://react.dev/)** - UI library
- **[TypeScript](https://www.typescriptlang.org/)** - Type-safe JavaScript
- **[Three.js r128](https://threejs.org/)** - 3D graphics library
- **[Vite](https://vitejs.dev/)** - Fast build tool and dev server

### Additional Libraries
- **[Lucide React](https://lucide.dev/)** - Beautiful icon set
- **Custom CSS** - No heavy CSS frameworks, lightweight styling

## 📜 Available Scripts

### Development
```bash
npm run dev
```
Starts the development server at `http://localhost:3000`

### Build
```bash
npm run build
```
Creates an optimized production build in the `dist/` folder

### Preview
```bash
npm run preview
```
Preview the production build locally

## 🎨 Customization

### Adding More Countries

Edit the `countries` array in `src/App.tsx`:

```typescript
const countries: Country[] = [
  { name: 'Your Country', lat: 12.34, lng: 56.78 },
  // Add more countries...
];
```

### Changing Globe Appearance

Modify the globe material in `src/App.tsx`:

```typescript
const material = new THREE.MeshPhongMaterial({
  color: 0x2233aa,        // Ocean color
  emissive: 0x112244,     // Glow color
  specular: 0x333333,     // Reflection
  shininess: 25,          // Glossiness
});
```

### Styling

All styles are in `src/styles.css`. Modify colors, spacing, and layouts to match your preferences.

## 🔮 Future Enhancements

- [ ] **Persistent Storage** - Save photos to localStorage or database
- [ ] **More Countries** - Expand to 100+ destinations
- [ ] **Search Function** - Find countries quickly
- [ ] **Real Globe Textures** - Add Earth texture maps
- [ ] **Travel Routes** - Draw lines between visited countries
- [ ] **Social Sharing** - Share your travel map
- [ ] **Mobile App** - Native iOS/Android version
- [ ] **Cloud Sync** - Sync photos across devices

## 📝 Notes

- Photos are stored in browser memory and will be lost on page refresh
- For persistent storage, consider implementing localStorage or a backend
- Three.js is loaded via CDN (defined in `public/index.html`)
- Supported image formats: JPG, PNG, GIF, WebP

## 🤝 Contributing

Contributions are welcome! Feel free to:
1. Fork the project
2. Create a feature branch (`git checkout -b feature/Feature`)
3. Commit your changes (`git commit -m 'Add some Feature'`)
4. Push to the branch (`git push origin feature/Feature`)
5. Open a Pull Request

## 📄 License

This project is licensed under the MIT License - see below for details:

```
MIT License

Copyright (c) 2024 Travel Memory Globe

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## 👤 Author

Ryan Beattie

## 🙏 Acknowledgments

- Three.js community for excellent 3D graphics library
- React team for the amazing framework
- Lucide for beautiful icons
- All travelers who inspired this project

---


**🐛 Found a bug?** Open an issue on GitHub

**💡 Have an idea?** We'd love to hear your suggestions!

Made with ❤️ for travelers around the world 🌍✈️📸
