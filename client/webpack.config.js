const path = require('path');
const MiniCssExtractPlugin = require('mini-css-extract-plugin');
const HtmlWebpackPlugin = require('html-webpack-plugin');

module.exports = {
    entry: './src/index.ts',
    plugins: [
        new MiniCssExtractPlugin({
            filename: "style.css",
            chunkFilename: "[name].css"
        }),
        new HtmlWebpackPlugin({
            template: './src/index.ejs',
            // favicon: './src/assets/favicon.ico',  // CUSTOM NOTE Remove "extra line" that gets appended to head element of inst/www/index.html. That "extra line" is `<link rel="icon" href="favicon.ico">`
            minify: 'auto',
        }),
    ],
    module: {
        rules: [
            {
                test: /\.tsx?$/,
                use: 'ts-loader',
                exclude: /node_modules/,
            },
            {
                test: /\.(scss|css)$/,
                use: [MiniCssExtractPlugin.loader, 'css-loader', 'sass-loader'],
            },
            {
                test: /\.(png|svg|jpg|jpeg|gif)$/i,
                type: 'asset/resource',
                exclude: /icons/,
            },
            { 
                test: /icons.*\.svg$/i, 
                loader: 'svg-inline-loader' 
            },
            {
                test: /\.ejs$/,
                use: {
                    loader: 'ejs-compiled-loader',
                    options: {
                        htmlmin: true,
                        htmlminOptions: {
                            removeComments: true
                        }
                    }
                }
            },
        ],
    },
    resolve: {
        extensions: ['.tsx', '.ts', '.js'],
    },
    output: {
        hashFunction: "xxhash64",
        filename: 'bundle.js',
        path: path.resolve(__dirname, '../inst/www'),
        clean: true,
    },
    devServer: {
        static: path.join(__dirname, '../inst/www'),
        compress: true,
        port: 9000,
    },
};
