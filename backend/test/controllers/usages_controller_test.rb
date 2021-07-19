require 'test_helper'

class UsagesControllerTest < ActionDispatch::IntegrationTest
  setup do
    @usage = usages(:one)
  end

  test "should get index" do
    get usages_url, as: :json
    assert_response :success
  end

  test "should create usage" do
    assert_difference('Usage.count') do
      post usages_url, params: { usage: { device_id: @usage.device_id } }, as: :json
    end

    assert_response 201
  end

  test "should show usage" do
    get usage_url(@usage), as: :json
    assert_response :success
  end

  test "should update usage" do
    patch usage_url(@usage), params: { usage: { device_id: @usage.device_id } }, as: :json
    assert_response 200
  end

  test "should destroy usage" do
    assert_difference('Usage.count', -1) do
      delete usage_url(@usage), as: :json
    end

    assert_response 204
  end
end
