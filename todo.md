# To-do
### Urgent for demo

  [x] Start full screen
  [x] In portrait layer, when mask activated, layer masking activate
  [x] Show brush for mask, rectangle . mousex and y tzpo . snap to grid according tp size , mouse wheel changes brushsize
  [x] AI collage - snaps to last portrait transform
  [x] edit global on ai collage 
  [x] grid openframeworks . fake histo - 
  [x] CPU perf by default 
  [x] Data under mouse

### General
  [ ] Animation output.
  [x] Add size presets
  [ ] Add app icon

### Layer System
  [x] Sort out Fbo flipping.
  [x] Center and fit layer?
  [x] Filters don't redraw on reorder?
  [ ] Add Mask Sharing
  [x] Add canvas size presets
  [ ] Make mask fbos GL_L

### GUI
  [ ] Better "New layer" interface
  [ ] Tooltips
  [ ] Meaningful figures on monitor
  [x] Add face extractor tool

### Static Layers
  [x] Folder face replace
  [ ] Add video layer
  [ ] Add fancy overlay for collages
  [ ] Fix manual collage (with drag and drop?)
  [x] Add paint layer


### Filter Layers
  [ ] Add banding to Alpha replace
  [ ] Add face replace layer
  [ ] Add PixelSorting Layer
  [ ] Look into alpha blending for image layers
  [x] Change default settings in glitch distort layer
  [x] Change default alpha cutoff for collage rendering
 
### Project Manager
  [ ] Add bang out for saving
  [ ] Correct texture scaling in preview
  [x] Add collage preset
  [ ] Add loading bar for assets
 
 ### BUGS
  [x] Mask tried to draw after layer deleted
  [x] Glitch distort has empty texture on first mark
  [x] Mouse position does not map to fbo
  [!] Generative collage blank fbos
  [x] MPEG Glitch doesn't resize
  [x] Histogram takes wrong texture back/front sometimes
  [x] Histogram not rendering properly in UI
  [x] Saving appears to be broken
  [x] Facedetector layers crash when deleted
  [x] Facedetector crashes sometimes;
  [x] Filter layers show mask when redrawing
  [x] Generative collage sometime has blank patches
  [x] Can't delete generative collage during build up
  [ ] Cursor location slightly off dureing selection
  [x] Manual collage cycling
  [ ] AI COllage sometime not selectable
  [x] Manual collage start a bit wrong...