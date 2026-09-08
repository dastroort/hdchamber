/* HDchamber is an interactive environment where geometric figures from
higher dimensions are brought to life and can be studied.
Copyright (C) 2024  Davide Garbato (deeglome)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#pragma once
#include<cstddef>
#include<vector>
#include<Eigen/Dense>

namespace Hyper
{
    /**
     * @class HyperCam
     * @brief Represents the $n$-dimensional generalization of a rendering camera that always looks at the origin.
     *
     * This class manages cameras using hyperspherical coordinates relative to the origin of an $n$-dimensional vector space.
     * It also keeps track of the matrix representing the camera’s transformation from its canonical position to its current
     * position, which is used to render vectors in $n-1$ dimensions.
     *
     * @note Be sure to call `update_cam_matrix()` if you have changed the camera’s position before performing any other operations.
     * @author Davide Garbato
     * @date September 2026
     */
    class HyperCam
    {
        public:
            /**
             * @brief Constructs a HyperCam in the given ambient dimension.
             *
             * @param[in] ambient_dim The dimension of the embedding space the camera lives in.
             * @param[in] hyperspherical_pos Optional initial hyperspherical position of the camera.
             *            Defaults to an empty vector.
             */
            HyperCam(size_t ambient_dim, const std::vector<float>& hyperspherical_pos = {});

            /**
             * @brief Get the ambient dimension of the camera.
             *
             * @return The dimension of the embedding space in which the camera is defined.
             */
            size_t get_ambient_dim() const { return ambient_dim; }

            /**
             * @brief Get render dimension of the camera
             * 
             * @returns ambient_dim - 1
             * 
             * @see ambient_dim
             */
            size_t get_render_dim() const { return ambient_dim-1; }

            /**
             * @brief Get the camera's distance from the origin.
             *
             * @return The radial component (first coordinate $\rho$) of the camera's hyperspherical position.
             */
            float get_cam_distance() const { return hyperspherical_pos[0]; }

            /**
             * @brief Get the camera's current hyperspherical position.
             *
             * @return A const reference to the vector of hyperspherical coordinates describing the camera's position.
             */
            const std::vector<float>& get_hyperspherical_pos() const { return hyperspherical_pos; }

            /**
             * @brief Get the camera's transformation matrix.
             *
             * @return A copy of the matrix mapping the camera from its canonical position to its current position.
             *
             * @see cam_matrix
             */
            Eigen::MatrixXf get_cam_matrix() const { return cam_matrix; };

            /**
             * @brief Set the camera's hyperspherical position.
             *
             * @param[in] pos The new hyperspherical coordinates to assign to the camera.
             *
             * @note This does not automatically update the camera matrix; call `update_cam_matrix()` afterwards.
             */
            void set_hyperspherical_pos(const std::vector<float>& pos);

            /**
             * @brief Recomputes the camera's transformation matrix from its current hyperspherical position.
             *
             * Must be called after changing the camera's position via `set_hyperspherical_pos()`
             * before performing any rendering or matrix-dependent operations.
             */
            void update_cam_matrix();

            /**
             * @brief Renders a point from the camera's ambient dimension down to its render dimension.
             *
             * @param[in] p The point, expressed in the camera's ambient dimension, to be rendered.
             * @return Eigen::VectorXf The projected point in the camera's render dimension (ambient_dim - 1).
             *
             * @note Requires `update_cam_matrix()` to have been called after any position change.
             */
            Eigen::VectorXf render(const Eigen::VectorXf& p) const;

        private:
            /**
             * @brief Represents in which dimension the camera lives (embedded dimension).
             * 
             * The render dimension is $n-1$ where $n$ is `ambient_dim`. 
             */
            size_t ambient_dim;

            /**
             * @brief The camera's position expressed in hyperspherical coordinates.
             *
             * The first component represents the radial distance from the origin, while the
             * remaining components represent the angular coordinates, like the following:
             * \f[
             *   \mathbf{p} = \begin{bmatrix}
             *     \rho \\
             *     \theta \\
             *     \phi \\
             *     \psi_3 \\
             *      \psi_4 \\
             *     \vdots \\
             *     \psi_{n-1}
             *   \end{bmatrix}
             * \f]
             */
            std::vector<float> hyperspherical_pos;

            /**
             * @brief A matrix that maps the camera from its canonical position to its current position described by `hyperspherical_pos`.
             * 
             * The canonical position is representable by the following vector of
             * hyperspherical coordinates:
             * \f[
             *   \mathbf{p} = \begin{bmatrix}
             *     \rho \\
             *     0 \\
             *     \vdots \\
             *     0
             *   \end{bmatrix}
             * \f]
             * @note If $C$ is the matrix representing the camera position (`cam_matrix`), then:
             * \f[
             *   C^{-1} = C^T
             * \f]
             * This holds because it is a rotation matrix and, therefore, an orthogonal matrix.
             */
            Eigen::MatrixXf cam_matrix;
    };

    /**
     * @brief Represents a chain of cameras that sequentially project and render points across dimensions.
     * 
     * A `CamChain` is a pipeline of `HyperCam` instances where each camera performs a rendering 
     * from dimension $i$ down to $i-1$. The output of one camera is passed cascaded to the next 
     * one until the final target dimension is reached.
     */
    typedef std::vector<HyperCam> CamChain;

    /**
     * @brief Generates and initializes a camera chain for dimensional reduction and rendering.
     * 
     * This function constructs a `CamChain` that transitions the rendering process starting from 
     * an ambient dimension (\p from_ambient_dim) down to a target render dimension (\p to_render_dim). 
     * Each intermediate camera handles a single-step dimensional reduction:
     * \f[
     *   \text{AmbientDim} \; \rightarrow \; \ldots \; \rightarrow \; i \; \rightarrow \; i - 1 \; \; \rightarrow \; \ldots \; \rightarrow \text{RenderDim}
     * \f]
     * 
     * @param[in] from_ambient_dim The starting ambient dimension of the space.
     * @param[in] to_render_dim The final target dimension after the cascade.
     * @param[in] hyperspherical_pos_list Optional list of hyperspherical positions used to configure 
     *            the cameras along the chain. Defaults to an empty list.
     * @return CamChain A vector of `HyperCam` objects configured as a cascading pipeline.
     */
    CamChain get_cam_chain(const size_t from_ambient_dim, const size_t to_render_dim, std::vector<std::vector<float>> hyperspherical_pos_list = {});

    /**
     * @brief Updates the states and matrices of the cameras in the chain.
     * 
     * Iterates through the given \p cam_chain and updates the transformation matrices and internal 
     * rendering states for the cameras whose corresponding entry in \p dirty_flags is set to true.
     * 
     * @param[in,out] cam_chain Reference to the camera chain pipeline to be updated.
     * @param[in] dirty_flags A vector of boolean flags indicating which cameras in the chain require an update.
     * @return std::vector<size_t> A list of indices corresponding to the cameras that were successfully updated.
     */
    std::vector<size_t> update_cam_chain(CamChain& cam_chain, std::vector<bool>& dirty_flags);
}