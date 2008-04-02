#include <cassert>
#include "QuadratureRule.h"
#include "Numeric.h"

using namespace cigma;

// ---------------------------------------------------------------------------

QuadratureRule::QuadratureRule()
{
    meshPart = 0;
    points = 0;
    jxw = 0;
    basis_tab = 0;
    basis_jet = 0;
}

QuadratureRule::~QuadratureRule()
{
    if (points != 0) delete points;
    if (jxw != 0) delete [] jxw;
    if (basis_tab != 0) delete [] basis_tab;
    if (basis_jet != 0) delete [] basis_jet;
}


// ---------------------------------------------------------------------------

void QuadratureRule::set_mesh(MeshPart *mesh)
{
    assert(mesh != 0);
    assert(mesh->cell != 0);
    this->meshPart = mesh;
}

void QuadratureRule::set_quadrature(double *wts, double *pts, int npts, int ndim)
{
    /*
    assert(meshPart != 0);
    assert(meshPart->cell != 0);

    this->points = points;
    assert(points != 0);
    assert(points->n_points() > 0);
    assert(points->n_refdim() == meshPart->cell->n_celldim());
    */

    points = new QuadraturePoints();
    points->set_quadrature(wts, pts, npts, ndim);

    jxw = new double[npts];
}

void QuadratureRule::initialize_basis_tab()
{
    assert(meshPart != 0);
    assert(meshPart->cell != 0);
    assert(points != 0);
    assert(points->n_refdim() == meshPart->cell->n_celldim());
    
    const int nq = points->n_points();
    const int ndofs = meshPart->cell->n_nodes();

    // get shape function values at known quadrature points
    basis_tab = new double[nq * ndofs];
    meshPart->cell->shape(nq, points->qpts, basis_tab);
}

void QuadratureRule::initialize_basis_jet()
{
    assert(meshPart != 0);
    assert(meshPart->cell != 0);
    assert(points != 0);
    assert(points->n_refdim() == meshPart->cell->n_celldim());
    
    const int nq = points->n_points();
    const int ndofs = meshPart->cell->n_nodes();
    const int ndim = points->n_dim();

    // get shape function derivatives at known quadrature points
    basis_jet = new double[nq * ndofs * ndim];
    meshPart->cell->grad_shape(nq, points->qpts, basis_jet);
}

// ---------------------------------------------------------------------------

void QuadratureRule::update_jxw()
{
    Cell *cell = meshPart->cell;
    const int nq = points->n_points();
    for (int q = 0; q < nq; q++)
    {
        double jac[3][3];
        jxw[q] = points->weight(q) * cell->jacobian((*points)[q], jac);
    }
}

void QuadratureRule::select_cell(int e)
{
    // change active cell in meshPart (loads new cell vertices)
    meshPart->select_cell(e);

    // update jacobians
    update_jxw();

    // use new cell vertices to calculate global quadrature points
    // XXX: optional? -- consider case when mesh from field_{a,b} are identical
    points->apply_refmap(meshPart->cell);
}

// ---------------------------------------------------------------------------

double QuadratureRule::apply(Points &f)
{
    // assert(f.n_points() == points->n_points());
    
    double cell_total = 0.0;
    const int nq = f.n_points();
    const int valdim = f.n_dim();
    for (int q = 0; q < nq; q++)
    {
        for (int i = 0; i < valdim; i++)
        {
            cell_total += jxw[q] * f(q,i);
        }
    }
    return cell_total;
}

double QuadratureRule::L2(Points &f, Points &g)
{
    // XXX: this function would easily generalize into norm(f,g):
    // XXX: would need to change SQR(f_qi - g_qi) into norm_pow(norm_diff(f_qi,g_qi))

    // assert(f.n_points() == points->n_points());
    // assert(g.n_points() == points->n_points());

    double err = 0.0;
    const int nq = f.n_points();
    const int valdim = f.n_dim();
    for (int q = 0; q < nq; q++)
    {
        for (int i = 0; i < valdim; i++)
        {
            double f_qi = f(q,i);
            double g_qi = g(q,i);
            err += jxw[q] * SQR(f_qi - g_qi);
        }
    }
    return err;
}

// ---------------------------------------------------------------------------
