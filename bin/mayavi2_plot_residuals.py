#!/usr/bin/env python


filename = None

if __name__ == '__main__':

    import sys
    if len(sys.argv) != 2:
        print "Usage: %s residuals.vtk" % sys.argv[0]
        sys.exit(1)

    filename = sys.argv[1]


from enthought.mayavi.app import Mayavi

class ErrorPlot(Mayavi):

    def __init__(self, filename):
        self.filename = filename
        return

    def run(self):

        from enthought.mayavi.sources.vtk_file_reader import VTKFileReader
        from enthought.mayavi.modules.outline import Outline
        from enthought.mayavi.modules.surface import Surface
        from enthought.mayavi.filters.threshold import Threshold

        script = self.script
        
        script.new_scene()

        vtkfile = VTKFileReader()
        vtkfile.initialize(self.filename)
        script.add_source(vtkfile)
        
        outline = Outline()
        script.add_module(outline)

        threshold = Threshold()
        script.add_filter(threshold)

        surface = Surface()
        script.add_module(surface)

        return


if __name__ == '__main__':


    m = ErrorPlot(filename)
    m.main()
