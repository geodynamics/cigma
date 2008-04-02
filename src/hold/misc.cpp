
void compare(CompareCmd *env, FE_Field *field_a, PointField *field_b)
{
    // ....

    // dimensions
    int npts = field_b->points->n_points();
    int ndim = field_b->n_dim();
    int valdim = field_b->n_rank();

    // local values
    Points phi_a, phi_b;
    double *values_a = new double[valdim];
    double *values_b = new double[valdim];
    phi_a.set_data(values_a, 1, valdim);
    phi_b.set_data(values_b, 1, valdim);

    residuals->reset_accumulator();
    env->start_timer(npts,"pts");
    for (int i = 0; i < npts; i++)
    {
        double *globalPoint = &((field_b->points->data)[i*ndim]);

        field_a->eval(globalPoint, values_a);

        for (int j = 0; j < valdim; j++)
        {
            values_b[j] = field_b->values->data[i*valdim + j];
        }

        double err = qr->L2(phi_a, phi_b);
        residuals->update(i, err);
        env->update_timer(i);
    }
    env->end_timer();

    delete [] values_a;
    delete [] values_b;

    // ....
}


void FE::set_quadrature_points(QuadraturePoints *pts)
{
    assert(meshPart != 0);
    assert(meshPart->cell != 0);

    this->QuadratureRule::set_quadrature_points(pts);

    assert(points != 0);
    const int nq = points->n_points();
    const int ndofs = meshPart->cell->n_nodes();

    // get shape function values at known quadrature points
    basis_tab = new double[nq * ndofs];
    meshPart->cell->shape(nq, points->qpts, basis_tab);

    // get shape function values at known quadrature points
    //basis_jet = new double[nq * ndofs * dim];
    //meshPart->cell->grad_shape(nq, quadrature->qpts, basis_jet);
}

